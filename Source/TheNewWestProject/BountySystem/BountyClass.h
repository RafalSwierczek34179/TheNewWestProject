// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StepClass.h"
#include "BountyClass.generated.h"

UCLASS()
class THENEWWESTPROJECT_API ABountyClass : public AActor
{
	GENERATED_BODY()
	
public:	
	ABountyClass();
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;

// --------------------------------

protected:
	bool Completed = false;

	UPROPERTY(EditDefaultsOnly, Category = "Bounty")
    	TArray<TSubclassOf<AStepClass>> StepsToSpawn;

	TArray<AStepClass*> MissionSteps;

	UPROPERTY(EditDefaultsOnly, Category = "Bounty")
	int RewardMoney;

	UPROPERTY(EditDefaultsOnly, Category = "Bounty")
	FString Title;
	
	UPROPERTY(EditDefaultsOnly, Category = "Bounty")
	FString Description;

	// Spawns mission steps, stores them in MissionSteps Array in order and assigns their delegates
	void SpawnSteps();

public:
	bool IsCompleted()
	{
		return Completed;
	}

	FString GetDescription()
	{
		return Description;
	}

	FString GetTitle()
	{
		return Title;
	}

	// Runs after a step is completed, moves onto the next step in Mission Steps or sets mission to completed state
	UFUNCTION()
	virtual void IncrementMissionStep();

	// Collect money in C++, anything else will be in BPs
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void CollectRewards();

	// Called by Bounty Director, replaces any steps to alter mission when a player completes a supporting bounty
	void UpdateMissionSteps(TMap<int, TSubclassOf<AStepClass>> ReplacementSteps);
	
};
