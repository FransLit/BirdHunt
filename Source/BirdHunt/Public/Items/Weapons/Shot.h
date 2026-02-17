
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shot.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class BIRDHUNT_API AShot : public AActor
{
	GENERATED_BODY()
	
public:	
	AShot();

protected:
	virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere)
    USphereComponent* CollisionComponent;

    UPROPERTY(VisibleAnywhere)
    UProjectileMovementComponent* ProjectileMovement;

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, FVector NormalImpulse,
        const FHitResult& Hit);

public:	
	virtual void Tick(float DeltaTime) override;
};



