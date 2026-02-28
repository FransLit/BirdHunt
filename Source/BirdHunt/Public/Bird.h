#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "NiagaraSystem.h"
#include "Bird.generated.h"

class USkeletalMeshComponent;
class UAnimInstance;
class AAKillWidgetActor;
class USoundBase;

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
	UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* Body;

	// Species
	UPROPERTY(EditDefaultsOnly, Category = "Species")
	TArray<USkeletalMesh*> SpeciesMeshes;

	// Species
	UPROPERTY(EditDefaultsOnly, Category = "Species")
	TArray<TSubclassOf<UAnimInstance>> SpeciesAnimInstances;

	UPROPERTY(EditAnywhere, Category = "Species")
	int32 SpeciesIndex = -1;

	// Movement
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MinWaitTime = 1.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MaxWaitTime = 10.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MinFlightHeight = 100.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MaxFlightHeight = 300.f;

	// VFX
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX")
	UNiagaraSystem* ShotEffect;

	bool bScared = false;

	float ScaredTimer = 0.f;

	void OnShot();
	void MoveToEscape(float DeltaTime);

	void IncrementScaredTimer(float increment);

	void SetScaredWithDelay();
	void SetScared();

	void TraceToGround();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bird|Movement")
	bool bDead = false;

	FVector GroundTargetLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bird|Movement")
	bool bFalling = false;

	UPROPERTY(EditAnywhere, Category = "Bird|Movement")
	float FallSpeed = 500.f;
	
	UPROPERTY(EditAnywhere, Category = "Bird|Movement")
	TSubclassOf<AAKillWidgetActor> KillWidgetActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	USoundBase* ShotSound;
private:
	int32 CurrentWaypointIndex = -1;
	bool bIsWaiting = false;

	FVector StartLocation;

	float WaitTimer = 0.f;
	float CurrentWaitTime = 0.f;

	float FlightAlpha = 0.f;
	float StartZ = 0.f;
	float HeightOffset = 0.f;

	void MoveToWaypoint(float DeltaTime);
	void ChooseNewRandomWaypoint(int32 WaypointCount);
};