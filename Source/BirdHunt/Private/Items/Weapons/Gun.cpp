
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

		TArray<AActor*> FoundBirds;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABird::StaticClass(), FoundBirds);

		for (AActor* Actor : FoundBirds)
		{
			ABird* Bird = Cast<ABird>(Actor);
			if (!Bird)
				continue;

			float Distance = FVector::Dist(Bird->GetActorLocation(), Location);

			
			if (Distance <= 3000.f)
			{
				Bird->bScared = true;
				Bird->ScaredTimer = 10.f;

				UE_LOG(LogTemp, Warning, TEXT("Bird scared: %s"), *Bird->GetName());
			}
		}

		
	}
}