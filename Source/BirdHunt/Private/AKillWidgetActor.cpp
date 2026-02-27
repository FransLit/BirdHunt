#include "AKillWidgetActor.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Components/WidgetComponent.h"
#include "KillBirdWidget.h"


AAKillWidgetActor::AAKillWidgetActor()
{
    PrimaryActorTick.bCanEverTick = true;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
    WidgetComp->SetupAttachment(RootComponent);
    WidgetComp->SetWidgetSpace(EWidgetSpace::World);
    WidgetComp->SetDrawSize(FVector2D(50.f, 50.f));
    WidgetComp->SetPivot(FVector2D(0.5f, 0.f));
}

void AAKillWidgetActor::BeginPlay()
{
    Super::BeginPlay();

    LifeTimer = 0.f;
}

void AAKillWidgetActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    LifeTimer += DeltaTime;

    // Float upward
    FVector NewLocation = GetActorLocation();
    NewLocation.Z += FloatSpeed * DeltaTime;
    SetActorLocation(NewLocation);

    // Face player
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
    {
        FVector CameraLoc = PC->PlayerCameraManager->GetCameraLocation();
        FVector WidgetLoc = GetActorLocation();
        FRotator LookRot = (CameraLoc - WidgetLoc).Rotation();
        WidgetComp->SetWorldRotation(LookRot);
    }

    // Fade out immediately
    if (WidgetComp)
    {
        if (UUserWidget* Widget = WidgetComp->GetUserWidgetObject())
        {
            float Alpha = 1.f - (LifeTimer / Lifetime);
            Alpha = FMath::Clamp(Alpha, 0.f, 1.f);
            Widget->SetRenderOpacity(Alpha);
        }
    }

    // Destroy after lifetime
    if (LifeTimer >= Lifetime)
    {
        Destroy();
    }
}

void AAKillWidgetActor::InitWidget(int32 KillCount)
{
    if (!WidgetComp) return;

    if (UUserWidget* Widget = WidgetComp->GetUserWidgetObject())
    {
        if (UKillBirdWidget* KillWidget = Cast<UKillBirdWidget>(Widget))
        {
            KillWidget->SetKillCount(KillCount);
        }
    }

    // Optional: rotate to player immediately
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
    {
        FVector CameraLoc = PC->PlayerCameraManager->GetCameraLocation();
        FVector WidgetLoc = GetActorLocation();
        FRotator LookRot = (CameraLoc - WidgetLoc).Rotation();
        WidgetComp->SetWorldRotation(LookRot);
    }
}