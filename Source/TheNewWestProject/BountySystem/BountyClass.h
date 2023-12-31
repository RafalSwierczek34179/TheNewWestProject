// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StepClass.h"
#include "BountyClass.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCompletedFirstStep);

UCLASS()
class THENEWWESTPROJECT_API ABountyClass : public AActor
{
	GENERATED_BODY()

	TSubclassOf<AStepClass> ReplacementStepClass;

	bool hasBeenStarted = false;
	
public:	
	ABountyClass();
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;

// --------------------------------

protected:
	bool Completed = false;

	/** 2 because the final step will always be to return to the
	 * ship which can't be completed, change this to 1 if final
	 * step isn't to return to ship
	*/
	int MinStepsRequiredForCompletion = 2;

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

	UFUNCTION(BlueprintImplementableEvent, Category = "Bounty")
	void StepCompletionFeedbackUI(const FString &NextStepDesc);

public:
	FCompletedFirstStep CompletedFirstStep;
	
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

	FVector GetStepLoc()
	{
		if (MissionSteps.IsEmpty() || MissionSteps[0] == nullptr)
		{
			return FVector(0, 0, 0);
		}

		return MissionSteps[0]->GetStepSpawnTransform().GetLocation();
	}

	FVector GetActiveWaypointLoc()
	{
		if (MissionSteps.IsEmpty() || MissionSteps[0] == nullptr)
		{
			return FVector(0, 0, 0);
		}
		return MissionSteps[0]->GetWaypointLoc();
	}

	UTexture2D* GetActiveWaypointIcon()
	{
		if (MissionSteps.IsEmpty() || MissionSteps[0] == nullptr)
		{
			return nullptr;
		}
		return MissionSteps[0]->GetWaypointIcon();
	}

	FString GetActiveStepDesc()
	{
		if (MissionSteps.IsEmpty() || MissionSteps[0] == nullptr)
		{
			return FString("Mission Description is Broken, GG. BountyClass.h GetActiveStepDesc()");
		}
		return MissionSteps[0]->GetStepDescription();
	}

	// Runs after a step is completed, moves onto the next step in Mission Steps or sets mission to completed state
	UFUNCTION()
	virtual void IncrementMissionStep();

	// Collect money in C++, anything else will be in BPs
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void CollectRewards();

	void DestroyReturnToShipStep()
	{
		if (MissionSteps.Num() > 1)
		{
			UE_LOG(LogTemp, Warning, TEXT("Didn't Destroy final step as more than one steps still left in Bounty"));
		}

		MissionSteps[0]->Destroy();
	}
	
	// Called by Bounty Director, replaces any steps to alter mission when a player completes a supporting bounty
	void UpdateMissionSteps(TMap<int, TSubclassOf<AStepClass>> ReplacementSteps);
	
};
