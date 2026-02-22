
#include "Items/Weapons/Gun.h"
#include "Bird.h"
#include "Kismet/GameplayStatics.h"
#include "Items/Weapons/Shot.h"


AGun::AGun()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(RootComponent);
}

void AGun::Reload()
{
    ShotsRemaining = 1;
    bReloading = false;
    UE_LOG(LogTemp, Warning, TEXT("Reload finished"));
}

void AGun::StartReloading()
{
    FTimerHandle TimerHandle;

    bReloading = true;

    UE_LOG(LogTemp, Warning, TEXT("Reloading"));

    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle,
        [this]()
        {
            Reload();
        },
        ReloadTime,
        false
    );
}

void AGun::BeginPlay()
{
	Super::BeginPlay();
}

void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::PullTrigger()
{
    if (!ProjectileClass || ShotsRemaining == 0) return;

    UE_LOG(LogTemp, Warning, TEXT("FIRE"));

    FVector Location = MuzzleLocation->GetComponentLocation();
    FVector Forward = MuzzleLocation->GetForwardVector();

    float SpreadRad = FMath::DegreesToRadians(SpreadAngle);

    for (int32 i = 0; i < PelletCount; i++)
    {
        FVector ShotDirection = FMath::VRandCone(Forward, SpreadRad);
        FRotator ShotRotation = ShotDirection.Rotation();

        GetWorld()->SpawnActor<AShot>(ProjectileClass, Location, ShotRotation);
    }
    ShotsRemaining = 0;

    TArray<AActor*> FoundBirds;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABird::StaticClass(), FoundBirds);

    for (AActor* Actor : FoundBirds)
    {
        ABird* Bird = Cast<ABird>(Actor);
        if (!Bird) continue;

        float Distance = FVector::Dist(Bird->GetActorLocation(), Location);

        if (Distance <= 3000.f)
        {
            Bird->SetScaredWithDelay();
            Bird->IncrementScaredTimer(5 + FMath::FRandRange(0.0f, 5.0f));
            UE_LOG(LogTemp, Warning, TEXT("Bird scared: %s %f"), *Bird->GetName(), Bird->ScaredTimer);
        }
    }
}