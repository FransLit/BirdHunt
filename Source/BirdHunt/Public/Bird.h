#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Curves/CurveFloat.h"
#include "Bird.generated.h"

UCLASS()
class BIRDHUNT_API ABird : public AActor
{
	GENERATED_BODY()

public:
	ABird();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// Components
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Body;

	// ================= SPECIES SYSTEM =================
	// Assign 6 different meshes in editor
	UPROPERTY(EditDefaultsOnly, Category = "Species")
	TArray<UStaticMesh*> SpeciesMeshes;

	// Which species this bird is
	UPROPERTY(VisibleAnywhere, Category = "Species")
	int32 SpeciesIndex = -1;

	// ================= MOVEMENT =================
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MinWaitTime = 1.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MaxWaitTime = 10.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MinFlightHeight = 100.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MaxFlightHeight = 300.f;

	void MoveToEscape(float DeltaTime);
	void OnShot();

	bool bScared = false;

private:
	int32 CurrentWaypointIndex = -1;
	float TravelTime = 0.f;
	bool bIsWaiting = false;

	UPROPERTY()
	FVector StartLocation;

	float WaitTimer = 0.f;
	float CurrentWaitTime = 0.f;

	float FlightAlpha = 0.f;
	float StartZ = 0.f;
	float HeightOffset = 0.f;

	void MoveToWaypoint(float DeltaTime);
	void ChooseNewRandomWaypoint(int32 WaypointCount);
};
