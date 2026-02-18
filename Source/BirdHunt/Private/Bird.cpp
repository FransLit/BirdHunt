#include "Bird.h"
#include "Framework/BirdHuntGameMode.h"
#include "EscapePoint.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ABird::ABird()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(RootComponent);
}

void ABird::BeginPlay()
{
	Super::BeginPlay();

	ABirdHuntGameMode* GM = Cast<ABirdHuntGameMode>(UGameplayStatics::GetGameMode(this));
	if (GM && GM->Waypoints.Num() > 0)
	{
		ChooseNewRandomWaypoint(GM->Waypoints.Num());
	}
}

void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveToWaypoint(DeltaTime);
}
void ABird::MoveToEscape(float DeltaTime)
{
	if (!bScared) return;

	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), AEscapePoint::StaticClass());
	if (!FoundActor) return;

	AEscapePoint* EP = Cast<AEscapePoint>(FoundActor);
	if (!EP) return;

	FVector TargetLocation = EP->GetActorLocation();
	FVector CurrentLocation = GetActorLocation();

	if (bIsWaiting)
	{
		WaitTimer += DeltaTime;

		// Level bird while waiting
		FRotator Rot = GetActorRotation();
		Rot.Pitch = 0.f;
		Rot.Roll = 0.f;
		SetActorRotation(Rot);

		if (WaitTimer >= CurrentWaitTime)
		{
			bIsWaiting = false;
			WaitTimer = 0.f;

			bScared = false;
			ChooseNewRandomWaypoint(Cast<ABirdHuntGameMode>(UGameplayStatics::GetGameMode(this))->Waypoints.Num());
		}
		return;
	}

	if (FlightAlpha <= 0.f)
	{
		StartLocation = GetActorLocation();
		StartZ = StartLocation.Z;
		HeightOffset = FMath::RandRange(MinFlightHeight, MaxFlightHeight);
	}

	float FixedSpeed = 1000.f;
	float Distance = (TargetLocation - StartLocation).Size();
	if (Distance > KINDA_SMALL_NUMBER)
	{
		FlightAlpha += (FixedSpeed * DeltaTime) / Distance;
	}
	FlightAlpha = FMath::Clamp(FlightAlpha, 0.f, 1.f);

	FVector FlatStart = StartLocation;
	FVector FlatTarget = TargetLocation;
	FlatStart.Z = 0.f;
	FlatTarget.Z = 0.f;
	FVector NewLocation = FMath::Lerp(FlatStart, FlatTarget, FlightAlpha);

	float LinearZ = FMath::Lerp(StartZ, TargetLocation.Z, FlightAlpha);
	float AltitudeDifference = FMath::Abs(TargetLocation.Z - StartZ);
	float ArcHeight = FMath::Max(HeightOffset, AltitudeDifference);
	float ArcZ = 4.f * ArcHeight * FlightAlpha * (1.f - FlightAlpha);
	NewLocation.Z = LinearZ + ArcZ;

	SetActorLocation(NewLocation);

	// --- Rotate with pitch while moving, level while waiting ---
	FRotator NewRotation;
	if (bIsWaiting)
	{
		NewRotation = GetActorRotation();
		NewRotation.Pitch = 0.f;
		NewRotation.Roll = 0.f;
	}
	else
	{
		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation);
		NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 5.f);
		NewRotation.Roll = 0.f; // keep roll level
	}
	SetActorRotation(NewRotation);

	if (FlightAlpha >= 1.f)
	{
		bIsWaiting = true;
		CurrentWaitTime = FMath::RandRange(MinWaitTime, MaxWaitTime);
		FlightAlpha = 0.f;
	}
}



void ABird::OnShot()
{
	// Silloin, kun lintua ammutaan



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
	TravelTime = 0.f;

	HeightOffset = FMath::RandRange(MinFlightHeight, MaxFlightHeight);
}
void ABird::MoveToWaypoint(float DeltaTime)
{
	if (!bScared)
	{
		ABirdHuntGameMode* GM = Cast<ABirdHuntGameMode>(UGameplayStatics::GetGameMode(this));
		if (!GM || !GM->Waypoints.IsValidIndex(CurrentWaypointIndex)) return;

		AActor* TargetWaypoint = GM->Waypoints[CurrentWaypointIndex];
		if (!TargetWaypoint) return;

		if (bIsWaiting)
		{
			WaitTimer += DeltaTime;

			// Level bird while waiting
			FRotator Rot = GetActorRotation();
			Rot.Pitch = 0.f;
			Rot.Roll = 0.f;
			SetActorRotation(Rot);

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

		TravelTime += DeltaTime;

		float FixedSpeed = 700.f;

		float Distance = (TargetLocation - StartLocation).Size();
		if (Distance > KINDA_SMALL_NUMBER)
		{
			FlightAlpha += (FixedSpeed * DeltaTime) / Distance;
		}
		FlightAlpha = FMath::Clamp(FlightAlpha, 0.f, 1.f);

		FVector FlatStart = StartLocation;
		FVector FlatTarget = TargetLocation;
		FlatStart.Z = 0.f;
		FlatTarget.Z = 0.f;

		FVector NewLocation = FMath::Lerp(FlatStart, FlatTarget, FlightAlpha);

		float LinearZ = FMath::Lerp(StartZ, TargetLocation.Z, FlightAlpha);
		float AltitudeDifference = FMath::Abs(TargetLocation.Z - StartZ);
		float ArcHeight = FMath::Max(HeightOffset, AltitudeDifference);
		float ArcZ = 4.f * ArcHeight * FlightAlpha * (1.f - FlightAlpha);
		NewLocation.Z = LinearZ + ArcZ;

		if (FlightAlpha >= 1.f)
		{
			NewLocation = TargetLocation;
		}

		SetActorLocation(NewLocation);

		// --- Rotate with pitch while moving, level while waiting ---
		FRotator NewRotation;
		if (bIsWaiting)
		{
			NewRotation = GetActorRotation();
			NewRotation.Pitch = 0.f;
			NewRotation.Roll = 0.f;
		}
		else
		{
			FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation);
			NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 5.f);
			NewRotation.Roll = 0.f; // keep roll level
		}
		SetActorRotation(NewRotation);

		if (FlightAlpha >= 1.f)
		{
			bIsWaiting = true;
			CurrentWaitTime = FMath::RandRange(MinWaitTime, MaxWaitTime);
		}
	}
}
