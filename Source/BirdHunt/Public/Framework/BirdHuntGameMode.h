#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BirdHuntGameMode.generated.h"

class ABird;

UCLASS()
class BIRDHUNT_API ABirdHuntGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABirdHuntGameMode();

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<ABird> BirdClass;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	int32 BirdCount = 3;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TArray<AActor*> Waypoints;

	void RegisterShot(int32 SpeciesIndex);

protected:
	virtual void BeginPlay() override;

private:
	TArray<int32> SpeciesShotCounts;
};