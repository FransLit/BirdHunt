#include "Framework/BirdHuntGameMode.h"
#include "Bird.h"
#include "Waypoint.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

ABirdHuntGameMode::ABirdHuntGameMode()
{

    DefaultPawnClass = nullptr;
}

void ABirdHuntGameMode::BeginPlay()
{
    Super::BeginPlay();


   
    Waypoints.Empty();
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaypoint::StaticClass(), FoundActors);
    for (AActor* Actor : FoundActors)
    {
        if (AWaypoint* WP = Cast<AWaypoint>(Actor))
        {
            Waypoints.Add(WP);


            FVector Location = WP->GetActorLocation();
            UE_LOG(LogTemp, Warning, TEXT("Waypoint added: %s | Location: X=%.2f Y=%.2f Z=%.2f"),
                *WP->GetName(),
                Location.X,
                Location.Y,
                Location.Z);
        }
    }



    for (int32 i = 0; i < BirdCount; i++)
    {
        FVector SpawnLocation = FVector(FMath::RandRange(-500, 500), FMath::RandRange(-500, 500), FMath::RandRange(200, 400));
        FRotator SpawnRotation = FRotator::ZeroRotator;
        ABird* SpawnedBird = GetWorld()->SpawnActor<ABird>(BirdClass, SpawnLocation, SpawnRotation);
        if (SpawnedBird)
        {
            UE_LOG(LogTemp, Warning, TEXT("Bird spawned: %s"), *SpawnedBird->GetName());
        }
    }
}