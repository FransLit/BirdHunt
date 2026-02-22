
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

    UPROPERTY(EditDefaultsOnly, Category = "Shotgun")
    int32 PelletCount = 12;

    UPROPERTY(EditDefaultsOnly, Category = "Shotgun")
    float SpreadAngle = 5.f; // degrees

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shotgun")
    bool bReloading = false;

    void Reload();
    void StartReloading();

    int ShotsRemaining = 1;
    UPROPERTY(EditAnywhere, Category = "Shotgun")
    int ReloadTime = 2.f;

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




