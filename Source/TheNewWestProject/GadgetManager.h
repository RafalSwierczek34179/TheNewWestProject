// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gadget/GadgetBase.h"
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
	AGadgetBase* ReconGadget;
	AGadgetBase* CombatGadget;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gadget")
	TSubclassOf<AGadgetBase> ReconClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Gadget")
	TSubclassOf<AGadgetBase> CombatClass;
public:
	void SpawnGadgets(USceneComponent* PlayersRootComp);

	bool IsItSafeToEquip();

	void EquipRecon()
	{
		ReconGadget->Equip();
	}
	void EquipCombat()
	{
		CombatGadget->Equip();
	}
	

	
};
