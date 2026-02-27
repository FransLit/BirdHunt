// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BHPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Player/BHPlayerController.h"
#include "Player/UI/PlayerHUD.h"

#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/BirdHuntGameMode.h"
#include "Bird.h"



void ABHPlayerState::OnRepSpeciesShotCounts()
{
    APawn* Pawn = GetPawn();
    if (!Pawn) return;

    ABHPlayerController* PC = Cast<ABHPlayerController>(Pawn->GetController());
    if (!PC) return;

    if(PC && PC->HUDWidget)
    {
        PC->HUDWidget->UpdateScore(CalculateAllKills(SpeciesShotCounts));
    }
}

void ABHPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ABHPlayerState, SpeciesShotCounts);
}

int CalculateAllKills(TArray<int32> array)
{
    int Total = 0;
    for (int32 Value : array)
    {
        Total += Value;
    }
    return Total;
}
