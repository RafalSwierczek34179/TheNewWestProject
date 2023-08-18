// Fill out your copyright notice in the Description page of Project Settings.


#include "BountyDirectorClass.h"
// ------Default AActor Functionality ---------
UBountyDirectorClass::UBountyDirectorClass()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBountyDirectorClass::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// -----------------------------------------
void UBountyDirectorClass::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerChar = Cast<ATheNewWestProjectCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (PlayerChar == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player character can't be loaded in BountyDirectorClass.cpp in BeginPlay()"));
		return;
	}

	CurrentBountyIndex = 0;
	for (int Index = 0; Index < 3; Index++)
	{
		CurrentSBCIndexes.Add(Index);
	}

	// Spawn the first Bounty and supporting bounties
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (BC_Array.IsEmpty() || BC_Array[0] == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Bounties have been provided for Bounty Director"));
		return;
	}

	// Spawn first bounty, attach it to the player char, and update active bounties array for mission ui
	AActor* BountyActor = GetWorld()->SpawnActor<AActor>(BC_Array[CurrentBountyIndex], PlayerChar->GetActorLocation(), PlayerChar->GetActorRotation(), SpawnParams);
	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	BountyActor->AttachToActor(PlayerChar->GetOwner(), AttachmentRules);
	ActiveBC = Cast<ABountyClass>(BountyActor);
	if (ActiveBC == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ActiveBC can't be loaded in BountyDirectorClass.cpp in BeginPlay()"));
		return;
	}
	PlayerChar->ActiveBounties.Add(ActiveBC);

	// Spawn all supporting bounties for the main bounty and set their ActiveSBC_Index
	ActiveSBC.SetNum(3);
	int ActiveSBC_Index = 0;
	for (int SBC_Index : CurrentSBCIndexes)
	{
		// Skip if a supporting bounty hasn't been set up in this array slot
		if (SBC_Array[SBC_Index] == nullptr)
		{
			continue;
		}
		ASupportingBountyClass* SBC = Cast<ASupportingBountyClass>(GetWorld()->SpawnActor<AActor>(SBC_Array[SBC_Index], SpawnParams));
		SBC->SetActorTransform(SBC->GetSpawnTransform());
		ActiveSBC[ActiveSBC_Index] = SBC;
		SBC->ActiveSBC_Index = ActiveSBC_Index;
		ActiveSBC_Index++;
	}

	UpdateBountyDisplay();
	
}

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


