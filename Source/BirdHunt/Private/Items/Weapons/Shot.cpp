
#include "Items/Weapons/Shot.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Bird.h"
#include "Components/SphereComponent.h"
#include "Framework/BirdHuntGameMode.h"
#include "Kismet/GameplayStatics.h"




AShot::AShot()
{
    PrimaryActorTick.bCanEverTick = false;

    // Collision
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    CollisionComponent->InitSphereRadius(8.f);
    RootComponent = CollisionComponent;

    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionObjectType(ECC_GameTraceChannel1); // Pellet

    CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);

    // Overlap birds
    CollisionComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);

    // Block environment
    CollisionComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

    CollisionComponent->SetGenerateOverlapEvents(true);

    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AShot::OnOverlap);

    // Projectile Movement
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->InitialSpeed = 6000.f;
    ProjectileMovement->MaxSpeed = 6000.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->ProjectileGravityScale = 0.f;

    InitialLifeSpan = 3.0f;
}

void AShot::BeginPlay()
{
    Super::BeginPlay();

    //if (ProjectileMovement)
    //{
    //    FVector Forward = GetActorForwardVector();

    //    float SpreadRad = FMath::DegreesToRadians(SpreadAngle);

    //    FVector NewDirection = FMath::VRandCone(Forward, SpreadRad);

    //    ProjectileMovement->Velocity = NewDirection * ProjectileMovement->InitialSpeed;
    //}
}

void AShot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShot::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse,
    const FHitResult& Hit)
{
    if (OtherActor && OtherActor != this)
    {
        ABird* HitBird = Cast<ABird>(OtherActor);
        if (HitBird && !HitBird->bDead)
        {
            HitBird->OnShot(); 
            ABirdHuntGameMode* GM = Cast<ABirdHuntGameMode>(UGameplayStatics::GetGameMode(this));
            if (GM)
            {
                GM->RegisterShot(this->Owner, HitBird->SpeciesIndex);
            }
            Destroy();
        }
    }
}

void AShot::OnOverlap(UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this)
        return;

    ABird* HitBird = Cast<ABird>(OtherActor);
    if (HitBird && !HitBird->bDead)
    {
        HitBird->OnShot();

        ABirdHuntGameMode* GM = Cast<ABirdHuntGameMode>(UGameplayStatics::GetGameMode(this));
        if (GM)
        {
            GM->RegisterShot(GetOwner(), HitBird->SpeciesIndex);
        }

        Destroy();
    }
}
