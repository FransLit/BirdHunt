
#include "Items/Weapons/Shot.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Bird.h"
#include "Components/SphereComponent.h"

AShot::AShot()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    CollisionComponent->InitSphereRadius(8.f);
    RootComponent = CollisionComponent;

    CollisionComponent->SetCollisionObjectType(ECC_GameTraceChannel1); // Pellet

    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);

    // Block environment
    CollisionComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

    // Block birds (since ABird is likely WorldDynamic)
    CollisionComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);

    CollisionComponent->OnComponentHit.AddDynamic(this, &AShot::OnHit);

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
    Destroy();
    if (OtherActor && OtherActor != this)
    {
        ABird* HitBird = Cast<ABird>(OtherActor);
        if (HitBird)
        {
            HitBird->OnShot();   
        }
    }
}
