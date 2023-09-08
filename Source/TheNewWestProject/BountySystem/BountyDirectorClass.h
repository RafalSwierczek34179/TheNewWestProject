// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"
#include "BountyClass.h"
#include "SupportingBountyClass.h"
#include "TheNewWestProject/TheNewWestProjectCharacter.h"
#include "BountyDirectorClass.generated.h"


UCLASS()
class THENEWWESTPROJECT_API ABountyDirectorClass : public AActor
{
	GENERATED_BODY()
public:	
	ABountyDirectorClass();
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;

// --------------------------------------------

private:

// -------- Properties ---------
	ATheNewWestProjectCharacter* PlayerChar;
	
	// Ordered list of main bounties to spawn in
	UPROPERTY(EditDefaultsOnly, Category = "Bounty")
	TArray<TSubclassOf<ABountyClass>> BC_Array;

	/** Ordered list of supporting bounties, every 3 positions
	 * apply to 1 position in BC_Array. For example, pos 3, 4,
	 * and 5 in SBC_Array correspond to pos 1 in BC_Array
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Bounty")
	TArray<TSubclassOf<ASupportingBountyClass>> SBC_Array;

	// Array index of current main bounty
	int CurrentBountyIndex;
	
	// Array indexes of current supporting bounties
	TArray<int> CurrentSBCIndexes;
	
	// Currently active bounty
	UPROPERTY(VisibleAnywhere, Category = "Bounty")
	ABountyClass* ActiveBC;
	
	// Currently active supporting bounties
	UPROPERTY(VisibleAnywhere, Category = "Bounty")
	TArray<ASupportingBountyClass*> ActiveSBC;

	// Checks if completed current bounty and if so moves onto the next
	UFUNCTION(BlueprintCallable, Category = "Bounty")
	void UpdateBountyProgression();

// --------- Methods ---------

	// Collect reward for current bounty and prepare the next bounty
	void FinishActiveBC();
	

protected:	
	/** Blueprint implemented function called from c++, handles aesthetic changes to bounty display
	based on data from ActiveBC and ActiveSBC */
	UFUNCTION(BlueprintImplementableEvent, Category = "Bounty")
	void UpdateBountyDisplay();

	// A supporting bounty has been completed and wants to update the main bounties steps
	UFUNCTION()
	void SBC_Completed(int SBC_Index);

public:
	UFUNCTION(BlueprintCallable, Category = "Bounty")
	FString GetBountyTitle()
	{
		return ActiveBC == nullptr ? FString("N/A") : ActiveBC->GetTitle();
	}

	UFUNCTION(BlueprintCallable, Category = "Bounty")
	FString GetBountyDescription()
	{
		return ActiveBC == nullptr ? FString("N/A") : ActiveBC->GetDescription();
	}

	UFUNCTION(BlueprintCallable, Category = "Bounty")
	FString GetSBCTitle(int SBCIndex);

	UFUNCTION(BlueprintCallable, Category = "Bounty")
	FString GetSBCDesc(int SBCIndex);
};
