// Fill out your copyright notice in the Description page of Project Settings.


#include "SupportingBountyClass.h"

void ASupportingBountyClass::BeginPlay()
{
	Super::BeginPlay();

	MinStepsRequiredForCompletion = 1;
}


void ASupportingBountyClass::IncrementMissionStep()
{
	Super::IncrementMissionStep();
	if (Completed)
	{
		CompletedSBC_Delegate.Broadcast(ActiveSBC_Index);
	}
}

void ASupportingBountyClass::DestroySteps()
{
	for (int i = 0; i < MissionSteps.Num(); i++)
	{
		if (MissionSteps[i] == nullptr)
		{
			continue;
		}

		MissionSteps[i]->Destroy();
	}
}

