#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class AGun;


UCLASS()
class BIRDHUNT_API APlayerCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    APlayerCharacter();

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<AGun> GunClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    AGun* Gun;   // Name it whatever you want

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    USceneComponent* WeaponSlot;

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void StartJump();
    void StopJump();
    void StartSprint();
    void StopSprint();
    void Fire();

private:
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputMappingContext* PlayerMappingContext;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* MoveAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* LookAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* JumpAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* SprintAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* FireAction;

    float DefaultWalkSpeed;
    float SprintSpeed = 900.f;
};
