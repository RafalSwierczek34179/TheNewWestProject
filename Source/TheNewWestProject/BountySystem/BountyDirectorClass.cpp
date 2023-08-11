// Fill out your copyright notice in the Description page of Project Settings.


#include "BountyDirectorClass.h"
// ------Default AActor Functionality ---------
UBountyDirectorClass::UBountyDirectorClass()
{
	PrimaryComponentTick.bCanEverTick = true;
}
void UBountyDirectorClass::BeginPlay()
{
	Super::BeginPlay();
	
}
void UBountyDirectorClass::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// -----------------------------------------

void UBountyDirectorClass::FinishActiveBC()
{
	/**
	 *TODO:
	 *	CollectReward() from active Bounty Class
	 *	Destroy ActiveBC and ActiveSBC referenced actors
	 *	Increment CurrentBoutyIndex
	 *	Spawn in and store new ActiveBC and Active SBC
	 *	Update ActiveBounties[0] to be the new ActiveBC
	 *	Run UpdateBountyDisplay()
	 */
}

void UBountyDirectorClass::SBC_Completed(int SBC_Index)
{
	/**
	 *TODO:
	 * Store GetReplacementSteps() from ActiveSBC[SBC_Index]
	 * Run UpdateMissionSteps() in ActiveBC using the stored replacement steps
	 */
}


