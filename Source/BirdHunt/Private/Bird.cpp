#include "Bird.h"
#include "Framework/BirdHuntGameMode.h"
#include "EscapePoint.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"

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

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(CollisionBox);
	Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);


}

void ABird::BeginPlay()
{
	Super::BeginPlay();

	if (SpeciesMeshes.Num() > 0)
	{
		SpeciesIndex = FMath::RandRange(0, SpeciesMeshes.Num() - 1);
		Body->SetStaticMesh(SpeciesMeshes[SpeciesIndex]);
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

	if (bScared)
	{
		MoveToEscape(DeltaTime);

		ScaredTimer -= DeltaTime;
		if (ScaredTimer <= 0.f)
		{
			bScared = false;      // stop escaping
			ScaredTimer = 0.f;    // reset
		}
	}
	else
		MoveToWaypoint(DeltaTime);
}

void ABird::OnShot()
{
	if (ShotEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			ShotEffect,
			GetActorLocation(),
			GetActorRotation()
		);
	}

	ABirdHuntGameMode* GM = Cast<ABirdHuntGameMode>(UGameplayStatics::GetGameMode(this));
	if (GM)
	{
		GM->RegisterShot(SpeciesIndex);
	}

	Destroy();
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