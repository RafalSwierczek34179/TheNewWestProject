// Fill out your copyright notice in the Description page of Project Settings.


#include "StepClass.h"

// Sets default values
AStepClass::AStepClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStepClass::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStepClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStepClass::BroadcastCompletion()
{
	UE_LOG(LogTemp, Warning, TEXT("Delegate is being broadcast"));
	CompletedStepDelegate.Broadcast();
}


