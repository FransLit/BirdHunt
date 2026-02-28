// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AKillWidgetActor.generated.h"

class UUserWidget;
class UBirdKillWidget;
class UWidgetComponent;

UCLASS()
class BIRDHUNT_API AAKillWidgetActor : public AActor
{
    GENERATED_BODY()

public:
    AAKillWidgetActor();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    void InitWidget(int32 KillCount);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
    UWidgetComponent* WidgetComp;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    float Lifetime = 1.5f;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    float FloatSpeed = 50.f;

private:
    float LifeTimer = 0.f;
};