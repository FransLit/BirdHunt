#include "Framework/BirdHuntGameMode.h"
#include "Bird.h"
#include "Waypoint.h"
#include "Kismet/GameplayStatics.h"

ABirdHuntGameMode::ABirdHuntGameMode()
{
	DefaultPawnClass = nullptr;
}

void ABirdHuntGameMode::BeginPlay()
{
	Super::BeginPlay();

	Waypoints.Empty();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaypoint::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		Waypoints.Add(Actor);
	}

	// Initialize species counters
	if (BirdClass)
	{
		ABird* DefaultBird = BirdClass->GetDefaultObject<ABird>();
		if (DefaultBird)
		{
			SpeciesShotCounts.Init(0, DefaultBird->SpeciesMeshes.Num());
		}
	}

	// Spawn birds
	for (int32 i = 0; i < BirdCount; i++)
	{
		FVector SpawnLocation(
			FMath::RandRange(-500, 500),
			FMath::RandRange(-500, 500),
			FMath::RandRange(200, 400)
		);

		GetWorld()->SpawnActor<ABird>(BirdClass, SpawnLocation, FRotator::ZeroRotator);
	}
}

void ABirdHuntGameMode::RegisterShot(int32 SpeciesIndex)
{
	if (!SpeciesShotCounts.IsValidIndex(SpeciesIndex))
		return;

	SpeciesShotCounts[SpeciesIndex]++;

	ABird* DefaultBird = BirdClass->GetDefaultObject<ABird>();
	if (!DefaultBird) return;

	FString SpeciesName = TEXT("Unknown");

	if (DefaultBird->SpeciesMeshes.IsValidIndex(SpeciesIndex) &&
		DefaultBird->SpeciesMeshes[SpeciesIndex])
	{
		SpeciesName = DefaultBird->SpeciesMeshes[SpeciesIndex]->GetName();
	}

	UE_LOG(LogTemp, Warning,
		TEXT("Species %s shot! Total: %d"),
		*SpeciesName,
		SpeciesShotCounts[SpeciesIndex]
	);
}