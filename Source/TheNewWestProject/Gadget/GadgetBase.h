// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GadgetBP.h"
#include "GameFramework/Actor.h"
#include "GadgetBase.generated.h"

UCLASS()
class THENEWWESTPROJECT_API AGadgetBase : public AActor
{
// -------------------- Default ------------------------
	GENERATED_BODY()
public:	
	AGadgetBase();
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;
// -----------------------------------------------------	
private:
	APlayerController* PlayerController;	

	AGadgetBP* GadgetBP;
	
	bool GadgetOnCooldown = false;
	FTimerHandle CooldownHandle;

	bool GadgetInUse = false;
	FTimerHandle InUseHandle;
	
	void StartGadgetCoolDown();
	void ReadyGadget()
	{
		GadgetOnCooldown = true;
	};
	
	void StartInUseTimer();

	void CreateGadgetInput();
	void DestroyGadgetInput();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gadget")
	TSubclassOf<AGadgetBP> GadgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Gadget")
	FVector SpawnOffset = FVector(0, 0, 0);

	UPROPERTY(EditDefaultsOnly, Category = "Gadget")
	float GadgetMaxUpTime;
	UPROPERTY(EditDefaultsOnly, Category = "Gadget")
	float CooldownTime;

	// Starts InUse timer if gadget isn't on cooldown, override to implement gadget behaviour
	//virtual void Activate();
	// Starts Cooldown and automatically unequips gadget
	void FinishedUsing();

public:
	bool Equipped = false;

	void Equip();
	// Returns true if unequip succeeded
	bool Unequip();

	FVector GetSpawnOffset()
	{
		return SpawnOffset;
	}
};
