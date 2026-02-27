// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

class UTextBlock;

UCLASS()
class BIRDHUNT_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void ShowStats(ESlateVisibility state);
	void UpdateScore(int32 NewScore);
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ScoreText;
};
