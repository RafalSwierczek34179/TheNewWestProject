// Fill out your copyright notice in the Description page of Project Settings.


#include "VisionLink.h"

void AVisionLink::BeginPlay()
{
	Super::BeginPlay();
	
	this->SetActorTickEnabled(false);
}

void AVisionLink::Activate()
{
	
	if (GadgetCantBeActivated())
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("A Vision link Activate is ran"));
	Super::Activate();
	this->SetActorTickEnabled(true);
	
}

void AVisionLink::FinishedUsing()
{
	this->SetActorTickEnabled(false);

	Super::FinishedUsing();
}


void AVisionLink::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UE_LOG(LogTemp, Warning, TEXT("Tick is working on Vision Link"));
}

