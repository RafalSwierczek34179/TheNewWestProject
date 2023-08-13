// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "BountyClass.h"
#include "SupportingBountyClass.h"
#include "BountyDirectorClass.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THENEWWESTPROJECT_API UBountyDirectorClass : public USceneComponent
{
	GENERATED_BODY()
public:	
	UBountyDirectorClass();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	virtual void BeginPlay() override;

// --------------------------------------------

private:

// -------- Properties ---------	
	
	// Ordered list of main bounties to spawn in
	UPROPERTY(EditDefaultsOnly, Category = "Bounty")
	TArray<TSubclassOf<ABountyClass>> BC_Array;

	// Array index of current main bounty
	int CurrentBountyIndex;

	// Currently active bounty
	UPROPERTY(VisibleAnywhere, Category = "Bounty")
	ABountyClass* ActiveBC;

	// Currently active supporting bounties
	UPROPERTY(VisibleAnywhere, Category = "Bounty")
	TArray<ASupportingBountyClass*> ActiveSBC;

// --------- Methods ---------

	// Collect reward for current bounty and prepare the next bounty
	void FinishActiveBC();

protected:	
	/** Blueprint implemented function called from c++, handles aesthetic changes to bounty display
	based on data from ActiveBC and ActiveSBC */
	UFUNCTION(BlueprintImplementableEvent, Category = "Bounty")
	void UpdateBountyDisplay();

	// A supporting bounty has been completed and wants to update the main bounties steps
	void SBC_Completed(int SBC_Index);
};
