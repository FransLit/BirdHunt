#include "Bird.h"
#include "Framework/BirdHuntGameMode.h"
#include "EscapePoint.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"

#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

#include "Components/WidgetComponent.h"
#include "AKillWidgetActor.h"




ABird::ABird()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->SetBoxExtent(FVector(30.f));

	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);

	Body = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(CollisionBox);
	Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABird::BeginPlay()
{
	Super::BeginPlay();

	if (SpeciesMeshes.Num() > 0)
	{
		SpeciesIndex = FMath::RandRange(0, SpeciesMeshes.Num() - 1);
		if(SpeciesMeshes.IsValidIndex(SpeciesIndex))
		Body->SetSkeletalMesh(SpeciesMeshes[SpeciesIndex]);
		if (SpeciesAnimInstances.IsValidIndex(SpeciesIndex))
		Body->SetAnimInstanceClass(SpeciesAnimInstances[SpeciesIndex]);
	}

	ABirdHuntGameMode* GM = Cast<ABirdHuntGameMode>(UGameplayStatics::GetGameMode(this));
	if (GM && GM->Waypoints.Num() > 0)
	{
		ChooseNewRandomWaypoint(GM->Waypoints.Num());
	}
}

void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (bFalling)
	//{
	//	FVector CurrentLocation = GetActorLocation();
	//	FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, GroundTargetLocation, DeltaTime, FallSpeed);
	//	SetActorLocation(NewLocation);

	//	if (FVector::Dist(NewLocation, GroundTargetLocation) < 10.f)
	//	{
	//		bFalling = false;
	//	}
	//}
	if (bDead) return;

	if (bScared)
	{
		MoveToEscape(DeltaTime);

		ScaredTimer -= DeltaTime;
		if (ScaredTimer <= 0.f)
		{
			bScared = false;
			ScaredTimer = 0.f;


			StartLocation = GetActorLocation();
			FlightAlpha = 0.f;
		}
	}
	else
	{
		MoveToWaypoint(DeltaTime);
	}
}

void ABird::OnShot()
{

	FVector SpawnLoc = GetActorLocation() + FVector(0.f, 0.f, 100.f);

	AAKillWidgetActor* WidgetActor = GetWorld()->SpawnActor<AAKillWidgetActor>(
		KillWidgetActorClass,
		SpawnLoc,
		FRotator::ZeroRotator
	);

	if (WidgetActor)
	{
		WidgetActor->InitWidget(1); // show 1 bird killed
	}

	if (ShotEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			ShotEffect,
			GetActorLocation(),
			GetActorRotation()
		);
	}

	if (ShotSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			ShotSound,
			GetActorLocation()
		);
	}

	bDead = true;
	bFalling = true;
	//TraceToGround();

	CollisionBox->SetSimulatePhysics(true);
	//Body->SetSimulatePhysics(true);

	//Destroy();
	Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);


}

void ABird::ChooseNewRandomWaypoint(int32 WaypointCount)
{
	if (WaypointCount <= 0) return;

	int32 NewIndex;
	do
	{
		NewIndex = FMath::RandRange(0, WaypointCount - 1);
	} while (NewIndex == CurrentWaypointIndex && WaypointCount > 1);

	CurrentWaypointIndex = NewIndex;

	StartLocation = GetActorLocation();
	StartZ = StartLocation.Z;
	FlightAlpha = 0.f;
	HeightOffset = FMath::RandRange(MinFlightHeight, MaxFlightHeight);
}

void ABird::MoveToWaypoint(float DeltaTime)
{
	ABirdHuntGameMode* GM = Cast<ABirdHuntGameMode>(UGameplayStatics::GetGameMode(this));
	if (!GM || !GM->Waypoints.IsValidIndex(CurrentWaypointIndex)) return;

	AActor* TargetWaypoint = GM->Waypoints[CurrentWaypointIndex];
	if (!TargetWaypoint) return;

	if (bIsWaiting)
	{
		WaitTimer += DeltaTime;
		if (WaitTimer >= CurrentWaitTime)
		{
			bIsWaiting = false;
			WaitTimer = 0.f;
			ChooseNewRandomWaypoint(GM->Waypoints.Num());
		}
		return;
	}

	FVector CurrentLocation = GetActorLocation();
	FVector TargetLocation = TargetWaypoint->GetActorLocation();

	float Speed = 700.f;
	float Distance = (TargetLocation - StartLocation).Size();

	if (Distance > KINDA_SMALL_NUMBER)
		FlightAlpha += (Speed * DeltaTime) / Distance;

	FlightAlpha = FMath::Clamp(FlightAlpha, 0.f, 1.f);

	FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, FlightAlpha);

	float ArcZ = 4.f * HeightOffset * FlightAlpha * (1.f - FlightAlpha);
	NewLocation.Z += ArcZ;

	SetActorLocation(NewLocation);

	FRotator TargetRotation =
		UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation);

	SetActorRotation(
		FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 5.f)
	);

	if (FlightAlpha >= 1.f)
	{
		bIsWaiting = true;
		CurrentWaitTime = FMath::RandRange(MinWaitTime, MaxWaitTime);
	}
}

void ABird::MoveToEscape(float DeltaTime)
{
	AActor* FoundActor =
		UGameplayStatics::GetActorOfClass(GetWorld(), AEscapePoint::StaticClass());

	if (!FoundActor) return;

	FVector TargetLocation = FoundActor->GetActorLocation();
	FVector CurrentLocation = GetActorLocation();

	float Speed = 1000.f;
	FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();

	SetActorLocation(CurrentLocation + Direction * Speed * DeltaTime);

	FRotator TargetRotation =
		UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation);

	SetActorRotation(
		FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 5.f)
	);
}

void ABird::IncrementScaredTimer(float increment)
{
	ScaredTimer = ScaredTimer + increment;
	ScaredTimer = FMath::Clamp(ScaredTimer, 0, 10.f);
}

void ABird::SetScaredWithDelay()
{
	if (!GetWorld()) return;

	float Delay = FMath::FRandRange(1.0f, 5.0f);

	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		[this]()
		{
			bScared = true;
		},
		Delay,
		false
	);
}

void ABird::SetScared()
{
	bScared = true;
}



void ABird::TraceToGround()
{
	FVector Start = GetActorLocation();

	// Direction: mostly down (-Z), but slightly forward along actor's forward vector
	FVector ForwardXZ = GetActorForwardVector();
	ForwardXZ.Z = 0.f; // flatten to XZ plane if needed
	ForwardXZ.Normalize();

	float ForwardOffset = 200.f; // how far forward the trace goes
	float DownDistance = 10000.f; // how far down the trace goes

	FVector End = Start + ForwardXZ * ForwardOffset - FVector(0.f, 0.f, DownDistance);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); // ignore self

	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_WorldStatic, Params))
	{
		// Use X/Y offset towards forward direction, Z from ground hit
		GroundTargetLocation = FVector(Start.X + ForwardXZ.X * ForwardOffset,
			Start.Y + ForwardXZ.Y * ForwardOffset,
			Hit.Location.Z);

		bFalling = true;

		// Optional debug draw
		DrawDebugSphere(GetWorld(), GroundTargetLocation, 20.f, 12, FColor::Red, false, 5.f);
	}
}