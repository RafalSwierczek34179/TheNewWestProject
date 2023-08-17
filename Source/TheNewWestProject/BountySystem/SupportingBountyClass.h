// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BountyClass.h"
#include "SupportingBountyClass.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCompletedSBC, int, SBC_ActiveIndex);

UCLASS()
class THENEWWESTPROJECT_API ASupportingBountyClass : public ABountyClass
{
	GENERATED_BODY()

	// Stores new replacement steps for BC as well as what pos it will be replacing 
	UPROPERTY(EditDefaultsOnly, Category = "Supporting Bounty")
	TMap<int, TSubclassOf<AStepClass>> ReplacementSteps;

	FCompletedSBC CompletedSBC_Delegate;

public:
	// Set by Bounty Director, and used by director when deciding what SBC has been completed
	int ActiveSBC_Index;

	TMap<int, TSubclassOf<AStepClass>> GetReplacementSteps()
	{
		return ReplacementSteps;	
	}

	virtual void IncrementMissionStep() override;
	
	
};
