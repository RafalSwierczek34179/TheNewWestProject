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
	 *Assign every steps CompletedStep Delegate to IncrementMissionStep() function
	 */
}

void ABountyClass::IncrementMissionStep()
{
	/**
	 *TODO:
	 *Check if the Array has another step, if not set completed to true
	 *Else Destroy Current Step, shrink array, and set the new step in pos 0 to Active
	 */
}

void ABountyClass::CollectRewards_Implementation()
{
	/**
	 *TODO:
	 *Apply RewardMoney to player
	 */
}

void ABountyClass::UpdateMissionSteps(TMap<int, TSubclassOf<AStepClass>>)
{
	/**
	 *TODO:
	 *For every entry in the map:
	 *	Destroy current step at "int" index of Mission Steps Array
	 *	Spawn in new replacement step from the sub class
	 *	Store the new step ref at index "int" of Mission Steps
	 */
}


