// Fill out your copyright notice in the Description page of Project Settings.


#include "EscapePoint.h"

// Sets default values
AEscapePoint::AEscapePoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EscapePoint = CreateDefaultSubobject<USceneComponent>(TEXT("EscapePoint"));
	RootComponent = EscapePoint;

}

// Called when the game starts or when spawned
void AEscapePoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEscapePoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

