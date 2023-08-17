// Fill out your copyright notice in the Description page of Project Settings.


#include "SupportingBountyClass.h"

void ASupportingBountyClass::IncrementMissionStep()
{
	Super::IncrementMissionStep();
	if (Completed)
	{
		CompletedSBC_Delegate.Broadcast(ActiveSBC_Index);
	}
}
