#include "Framework/BirdHuntGameMode.h"
#include "Bird.h"
#include "Waypoint.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacter.h"
#include "Player/BHPlayerState.h"




ABirdHuntGameMode::ABirdHuntGameMode()
{
	DefaultPawnClass = nullptr;
}

void ABirdHuntGameMode::RegisterShot(AActor* owner, int32 SpeciesIndex)
{
	APlayerCharacter* Character = Cast<APlayerCharacter>(owner);
	if (!Character)
		return;

	APlayerState* PlayerState = Character->GetPlayerState();
	if (!PlayerState)
		return;

	ABHPlayerState* PS = Cast<ABHPlayerState>(PlayerState);
	if (PS)
	{
		UE_LOG(LogTemp, Error, TEXT("Shot a bird 1"))
		if(PS->SpeciesShotCounts.IsValidIndex(SpeciesIndex))
		{
			UE_LOG(LogTemp, Error, TEXT("Shot a bird 2"))
			PS->OnRepSpeciesShotCounts();
			PS->SpeciesShotCounts[SpeciesIndex] += 1;
		}
	}
	//ABird* DefaultBird = BirdClass->GetDefaultObject<ABird>();
	//if (!DefaultBird) return;

	//FString SpeciesName = TEXT("Unknown");

	//if (DefaultBird->SpeciesMeshes.IsValidIndex(SpeciesIndex) &&
	//	DefaultBird->SpeciesMeshes[SpeciesIndex])
	//{
	//	SpeciesName = DefaultBird->SpeciesMeshes[SpeciesIndex]->GetName();
	//}

	//UE_LOG(LogTemp, Warning,
	//	TEXT("Species %s shot! Total: %d"),
	//	*SpeciesName,
	//	SpeciesShotCounts[SpeciesIndex]
	//);
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


void ABirdHuntGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ABird* DefaultBird = BirdClass->GetDefaultObject<ABird>();
	if (!DefaultBird) return;

	int NumSpecies = DefaultBird->SpeciesMeshes.Num();
	if (NewPlayer)
	{
		ABHPlayerState* PS = Cast<ABHPlayerState>(NewPlayer->GetPlayerState<ABHPlayerState>());
		if (PS)
		{
			PS->SpeciesShotCounts.Init(0, NumSpecies);
		}
	}
}