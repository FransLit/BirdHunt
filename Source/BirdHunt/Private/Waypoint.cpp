// Fill out your copyright notice in the Description page of Project Settings.


#include "Waypoint.h"


AWaypoint::AWaypoint()
{

	PrimaryActorTick.bCanEverTick = true;


	TargetPoint = CreateDefaultSubobject<USceneComponent>(TEXT("TargetPoint"));
	RootComponent = TargetPoint;
}


void AWaypoint::BeginPlay()
{
	Super::BeginPlay();
	
}


void AWaypoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

