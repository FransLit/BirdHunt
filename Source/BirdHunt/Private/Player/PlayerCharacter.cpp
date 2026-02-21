#include "Player/PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Items/Weapons/Gun.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Math/UnrealMathUtility.h"

APlayerCharacter::APlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(Cast<USceneComponent>(GetCapsuleComponent()));
    Camera->SetRelativeLocation(FVector(0, 0, 60));
    Camera->bUsePawnControlRotation = true;

    WeaponSlot = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponSlot"));
    WeaponSlot->SetupAttachment(Camera);
}

void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    DefaultWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;

    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
                LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
            {
                Subsystem->AddMappingContext(PlayerMappingContext, 0);
            }
        }
    }

    if (GunClass)
    {
        Gun = GetWorld()->SpawnActor<AGun>(GunClass);

        if (Gun)
        {
            Gun->AttachToComponent(
                WeaponSlot,
                FAttachmentTransformRules::SnapToTargetIncludingScale
            );

            Gun->SetOwner(this);
        }
    }
}

void APlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bAiming)
    {
        /*
        * if (WeaponSlot->GetRelativeLocation().Y != 30.0f)
        {
            

                float y = DeltaTime * 0.1f + WeaponSlot->GetRelativeLocation().Y;
            WeaponSlot->SetRelativeLocation(Result);
        }
        */
        FVector Result = FMath::Lerp(WeaponSlot->GetRelativeLocation(), FVector(0, 30, -30), 0.1f);
        //float y = DeltaTime * 0.1f + WeaponSlot->GetRelativeLocation().Y;
        WeaponSlot->SetRelativeLocation(Result);
        UE_LOG(LogTemp, Warning, TEXT("NOT AIMING"));
    }
    else
    {
        FVector Result = FMath::Lerp(WeaponSlot->GetRelativeLocation(), FVector(0, 0, -30), 0.1f);
        WeaponSlot->SetRelativeLocation(Result);
        UE_LOG(LogTemp, Warning, TEXT("AIMING"));
    }
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
        EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
        EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::StartJump);
        EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopJump);
        EnhancedInput->BindAction(SprintAction, ETriggerEvent::Started, this, &APlayerCharacter::StartSprint);
        EnhancedInput->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopSprint);
        EnhancedInput->BindAction(FireAction, ETriggerEvent::Started, this, &APlayerCharacter::Fire);
        EnhancedInput->BindAction(AimAction, ETriggerEvent::Started, this, &APlayerCharacter::Aim);
    }
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();

    if (Controller)
    {
        FRotator Rotation = Controller->GetControlRotation();
        FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

        FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        AddMovementInput(Forward, MovementVector.Y);
        AddMovementInput(Right, MovementVector.X);
    }
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
    FVector2D LookAxis = Value.Get<FVector2D>();

    AddControllerYawInput(LookAxis.X);
    AddControllerPitchInput(LookAxis.Y);
}

void APlayerCharacter::StartJump()
{
    Jump();
}

void APlayerCharacter::StopJump()
{
    StopJumping();
}

void APlayerCharacter::StartSprint()
{
    GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void APlayerCharacter::StopSprint()
{
    GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
}

void APlayerCharacter::Fire()
{
    if(Gun)
    {
        Gun->PullTrigger();
    }
}
void APlayerCharacter::Aim()
{
    if (WeaponSlot)
    {
        bAiming = !bAiming;
    }
}