// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BHPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class BIRDHUNT_API ABHPlayerState : public APlayerState
{
	GENERATED_BODY()


public:
	UPROPERTY(ReplicatedUsing=OnRepSpeciesShotCounts)
	TArray<int32> SpeciesShotCounts;

	UFUNCTION()
	void OnRepSpeciesShotCounts();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};


int CalculateAllKills(TArray<int32> array);