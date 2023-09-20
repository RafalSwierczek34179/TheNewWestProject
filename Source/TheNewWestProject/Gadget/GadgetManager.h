// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatGadget.h"
#include "ReconGadget.h"
#include "GadgetBase.h"
#include "GameFramework/Actor.h"
#include "GadgetManager.generated.h"

UCLASS()
class THENEWWESTPROJECT_API AGadgetManager : public AActor
{
	// ------------- Defaults -------------------
	GENERATED_BODY()
public:	
	AGadgetManager();
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;
	// ------------------------------------------
private:
	AReconGadget* ReconGadget;
	ACombatGadget* CombatGadget;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gadget")
	TSubclassOf<AReconGadget> ReconClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Gadget")
	TSubclassOf<ACombatGadget> CombatClass;
public:
	void SpawnGadgets(USceneComponent* PlayersRootComp);
	
	void EquipRecon()
	{
		ReconGadget->Equip();
	}
	
	void EquipCombat()
	{
		CombatGadget->Equip();
	}
	
	bool IsReconEquipped()
	{
		return ReconGadget->Equipped;
	}
	
	bool IsCombatEquipped()
	{
		return CombatGadget->Equipped;
	}

	bool TryToUnequipRecon()
	{
		if (ReconGadget->Equipped)
		{
			return ReconGadget->Unequip();
		}
		return false;
	}

	bool TryToUnequipCombat()
	{
		if (CombatGadget->Equipped)
		{
			return CombatGadget->Unequip();
		}
		return false;
	}
	
};
