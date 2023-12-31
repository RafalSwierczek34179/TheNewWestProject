// Fill out your copyright notice in the Description page of Project Settings.


#include "BountyDirectorClass.h"
// ------Default AActor Functionality ---------
ABountyDirectorClass::ABountyDirectorClass()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABountyDirectorClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// -----------------------------------------
void ABountyDirectorClass::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerChar = Cast<ATheNewWestProjectCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (PlayerChar == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player character can't be loaded in BountyDirectorClass.cpp in BeginPlay()"));
		return;
	}

	// Setup a component on the player for bounties to attach to
    for (UActorComponent* comp : PlayerChar->GetComponentsByTag(UArrowComponent::StaticClass(), FName("Bounty")))
    {
    	BountyAttachmnetPoint = Cast<UArrowComponent>(comp);
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
	BountyActor->AttachToComponent(BountyAttachmnetPoint, AttachmentRules);
	ActiveBC = Cast<ABountyClass>(BountyActor);
	if (ActiveBC == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ActiveBC can't be loaded in BountyDirectorClass.cpp in BeginPlay()"));
		return;
	}
	PlayerChar->ActiveBounties.Add(ActiveBC);

	ActiveBC->CompletedFirstStep.AddDynamic(this, &ABountyDirectorClass::DestroyActiveSBCs);

	// Spawn all supporting bounties for the main bounty and set their ActiveSBC_Index
	ActiveSBC.SetNum(3);
	int ActiveSBC_Index = 0;
	for (int SBC_Index : CurrentSBCIndexes)
	{
		// Skip if a supporting bounty hasn't been set up in this array slot
		if (SBC_Array.Num() <= SBC_Index || SBC_Array[SBC_Index] == nullptr)
		{
			continue;
		}
		ASupportingBountyClass* SBC = Cast<ASupportingBountyClass>(GetWorld()->SpawnActor<AActor>(SBC_Array[SBC_Index], PlayerChar->GetActorLocation(), PlayerChar->GetActorRotation(),SpawnParams));
		SBC->AttachToComponent(BountyAttachmnetPoint, AttachmentRules);
		ActiveSBC[ActiveSBC_Index] = SBC;
		SBC->ActiveSBC_Index = ActiveSBC_Index;
		ActiveSBC[ActiveSBC_Index]->CompletedSBC_Delegate.AddDynamic(this, &ABountyDirectorClass::SBC_Completed);
		

		PlayerChar->ActiveBounties.Add(ActiveSBC[ActiveSBC_Index]);

		ActiveSBC_Index++;
	}

	UpdateBountyDisplay();
	
}

void ABountyDirectorClass::UpdateBountyProgression()
{
	if (ActiveBC == nullptr)
	{
		return;
	}

	if (ActiveBC->IsCompleted())
	{
		FinishActiveBC();
	}
}


void ABountyDirectorClass::FinishActiveBC()
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

	ActiveBC->CollectRewards();

	// Destroy old bounties
	ActiveBC->Destroy();
	ActiveBC->DestroyReturnToShipStep();
	DestroyActiveSBCs();

	// Increment Bounty Indexes
	CurrentBountyIndex++;
	for (int i = 0; i < 3; i++)
	{
		CurrentSBCIndexes[i] += 3;
	}

	if (CurrentBountyIndex >= BC_Array.Num())
	{
		// Finished all bounties, completed the game
		UE_LOG(LogTemp, Warning, TEXT("Finished all bounties, completed the game"));
		ActiveBC = nullptr;
		UpdateBountyDisplay();
		return;
	}

	if (BC_Array[CurrentBountyIndex] == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Bounty number %d hasn't been set, stop playing and set it"), CurrentBountyIndex);
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);

	// Spawn and setup main bounty
	AActor* BountyActor = GetWorld()->SpawnActor<AActor>(BC_Array[CurrentBountyIndex], PlayerChar->GetActorLocation(), PlayerChar->GetActorRotation(), SpawnParams);
	BountyActor->AttachToComponent(BountyAttachmnetPoint, AttachmentRules);
	ActiveBC = Cast<ABountyClass>(BountyActor);
	// Main bounty must always be the first on the players bounty list so other bounties can override it
	PlayerChar->ActiveBounties[0] = ActiveBC;

	ActiveBC->CompletedFirstStep.AddDynamic(this, &ABountyDirectorClass::DestroyActiveSBCs);
	
	// Spawn and setup supporting bounties
	int ActiveSBC_Index = 0;
	for (int SBC_Index : CurrentSBCIndexes)
	{
		// Skip if a supporting bounty hasn't been set up in this array slot
		if (SBC_Array.Num() <= SBC_Index || SBC_Array[SBC_Index] == nullptr)
		{
			continue;
		}
		ASupportingBountyClass* SBC = Cast<ASupportingBountyClass>(GetWorld()->SpawnActor<AActor>(SBC_Array[SBC_Index], PlayerChar->GetActorLocation(), PlayerChar->GetActorRotation(),SpawnParams));
		SBC->AttachToComponent(BountyAttachmnetPoint, AttachmentRules);
		ActiveSBC[ActiveSBC_Index] = SBC;
		SBC->ActiveSBC_Index = ActiveSBC_Index;
		ActiveSBC[ActiveSBC_Index]->CompletedSBC_Delegate.AddDynamic(this, &ABountyDirectorClass::SBC_Completed);
		

		PlayerChar->ActiveBounties.Add(ActiveSBC[ActiveSBC_Index]);

		ActiveSBC_Index++;
	}

	UpdateBountyDisplay();
}

void ABountyDirectorClass::DestroyActiveSBCs()
{
	for (ASupportingBountyClass* SBC: ActiveSBC)
	{
		if (SBC == nullptr)
		{
			continue;
		}
		
		PlayerChar->ActiveBounties.Remove(SBC);

		SBC->DestroySteps();
		SBC->Destroy();
	}

	UpdateBountyDisplay();
}


void ABountyDirectorClass::SBC_Completed(int SBC_Index)
{
	/**
	 *TODO:
	 * Store GetReplacementSteps() from ActiveSBC[SBC_Index]
	 * Run UpdateMissionSteps() in ActiveBC using the stored replacement steps
	 */

	ActiveBC->UpdateMissionSteps(ActiveSBC[SBC_Index]->GetReplacementSteps());

	PlayerChar->ActiveBounties.Remove(ActiveSBC[SBC_Index]);
	ActiveSBC[SBC_Index]->DestroySteps();
	ActiveSBC[SBC_Index]->Destroy();

	UpdateBountyDisplay();
}

FString ABountyDirectorClass::GetSBCTitle(int SBCIndex)
{
	if (SBCIndex > 2 || SBCIndex < 0 || ActiveSBC[SBCIndex] == nullptr)
	{
		return FString("");
	}

	return ActiveSBC[SBCIndex]->GetTitle();
}

FString ABountyDirectorClass::GetSBCDesc(int SBCIndex)
{
	if (SBCIndex > 2 || SBCIndex < 0 || ActiveSBC[SBCIndex] == nullptr)
	{
		return FString("");
	}

	return ActiveSBC[SBCIndex]->GetDescription();
}

