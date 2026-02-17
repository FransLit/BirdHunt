
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

class AShot;

UCLASS()
class BIRDHUNT_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	AGun();

    void PullTrigger();

protected:
	virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere)
    TSubclassOf<AShot> ProjectileClass;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* Root;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* MuzzleLocation;

public:	
	virtual void Tick(float DeltaTime) override;

};




