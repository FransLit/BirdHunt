
#include "Items/Weapons/Gun.h"
#include "Items/Weapons/Shot.h"


AGun::AGun()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(RootComponent);
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
	if (ProjectileClass)
	{
		UE_LOG(LogTemp, Error, TEXT("FIRE"))
		FVector Location = MuzzleLocation->GetComponentLocation();
		FRotator Rotation = MuzzleLocation->GetComponentRotation();
		GetWorld()->SpawnActor<AShot>(ProjectileClass, Location, Rotation);
	}
}