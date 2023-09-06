// Fill out your copyright notice in the Description page of Project Settings.


#include "BountyClass.h"

// Sets default values
ABountyClass::ABountyClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABountyClass::BeginPlay()
{
	Super::BeginPlay();

	SpawnSteps();
	
}

// Called every frame
void ABountyClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//-------------------------------------------

void ABountyClass::SpawnSteps()
{
	/**
	 *TODO:
	 *Spawn mission steps from StepsToSpawn array using their spawntransforms property
	 *Store spawned steps in completion order in MissionSteps array
	 *Assign first steps CompletedStep Delegate to IncrementMissionStep() function and set it to active
	 */

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	for (TSubclassOf<AStepClass> StepClass : StepsToSpawn)
	{
		if (StepClass == nullptr)
		{
			UE_LOG(LogTemp, Fatal, TEXT("Someone added a step slot without assigning it a step, assign it a step in %s Main Bounty's BP class defaults"), *Title);
			continue;
		}
		FVector Loc = StepClass.GetDefaultObject()->GetStepSpawnTransform().GetLocation();
		FRotator Rot = StepClass.GetDefaultObject()->GetStepSpawnTransform().GetRotation().Rotator();
		AStepClass* SpawnedStep = Cast<AStepClass>(GetWorld()->SpawnActor<AActor>(StepClass, Loc, Rot, SpawnParameters));
		MissionSteps.Add(SpawnedStep);		
	}

	// Allow the step to become active and listen for when it is completed
	MissionSteps[0]->Active = true;
	MissionSteps[0]->CompletedStepDelegate.AddDynamic(this, &ABountyClass::IncrementMissionStep);
}

void ABountyClass::IncrementMissionStep()
{
	/**
	 *TODO:
	 *Check if the Array has another step, if not set completed to true
	 *Else Destroy Current Step, shrink array, and set the new step in pos 0 to Active and apply delegate
	 */
	
	if (MissionSteps.Num() <= 1)
	{
		Completed = true;
		MissionSteps[0]->Active = false;
		MissionSteps[0]->Destroy();
		MissionSteps.RemoveAt(0);
		UE_LOG(LogTemp, Warning, TEXT("You've Completed the mission!!! Well done"));
		return;
	}

	if (MissionSteps[1] == nullptr)
	{
		UE_LOG(LogTemp, Fatal, TEXT("Missing step in Mission Steps, could be that it failed to spawn or cast"));
		return;
	}

	// Destroy Actor and shrink array
	MissionSteps[0]->Active = false;
	MissionSteps[0]->Destroy();
	MissionSteps.RemoveAt(0);

	// Set the new step in position 0 to active and assign its delegate
	MissionSteps[0]->Active = true;
	MissionSteps[0]->CompletedStepDelegate.AddDynamic(this, &ABountyClass::IncrementMissionStep);
	
}

void ABountyClass::CollectRewards_Implementation()
{
	/**
	 *TODO:
	 *Apply RewardMoney to player
	 */

	UE_LOG(LogTemp, Warning, TEXT("The player has gained $%d for completing the bounty!"), RewardMoney);
}

void ABountyClass::UpdateMissionSteps(TMap<int, TSubclassOf<AStepClass>> ReplacementSteps)
{
	/**
	 *TODO:
	 *For every entry in the map:
	 *	Destroy current step at "int" index of Mission Steps Array
	 *	Spawn in new replacement step from the sub class
	 *	Store the new step ref at index "int" of Mission Steps
	 */

	if (ReplacementSteps.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("No Replacement Steps found"));
		return;
	}
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (auto ReplacementStep : ReplacementSteps)
	{
		MissionSteps[ReplacementStep.Key]->Destroy();
		ReplacementStepClass = ReplacementStep.Value;

		FVector Loc = ReplacementStepClass.GetDefaultObject()->GetStepSpawnTransform().GetLocation();
		FRotator Rot = ReplacementStepClass.GetDefaultObject()->GetStepSpawnTransform().GetRotation().Rotator();
		AStepClass* SpawnedStep = Cast<AStepClass>(GetWorld()->SpawnActor<AActor>(ReplacementStep.Value, Loc, Rot, SpawnParameters));

		if (SpawnedStep == nullptr)
		{
			UE_LOG(LogTemp, Fatal, TEXT("The new step hasn't spawned in properly and the cast failed, honestly check all the key value stuff in UpdateMissionSteps in BountyClass.cpp"));
			return;
		}

		// DONT FORGET TO NOW USE THE NEW STEP TO REPLACE IT!!!!!!!!!

		MissionSteps[ReplacementStep.Key] = SpawnedStep;
		if (ReplacementStep.Key == 0)
		{
			MissionSteps[ReplacementStep.Key]->Active = true;
			MissionSteps[ReplacementStep.Key]->CompletedStepDelegate.AddDynamic(this, &ABountyClass::IncrementMissionStep);
		}
		
	}
}


