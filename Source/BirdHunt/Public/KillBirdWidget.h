// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KillBirdWidget.generated.h"

class UTextBlock;

UCLASS()
class BIRDHUNT_API UKillBirdWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable)
    void SetKillCount(int32 Count);

protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock* KillCountText;
};
