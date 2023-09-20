// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystemInterface.h"
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
	
	bool GadgetOnCooldown = false;
	FTimerHandle CooldownHandle;

	bool GadgetInUse = false;
	FTimerHandle InUseHandle;
	
	
	void ReadyGadget()
	{
		UE_LOG(LogTemp, Warning, TEXT("Gadget is now ready to be used again"));
		GadgetOnCooldown = false;
	};

protected:
	AGadgetBP* GadgetVisual;

	UPROPERTY(EditDefaultsOnly, Category = "Gadget")
	TSubclassOf<AGadgetBP> GadgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Gadget")
	FVector SpawnOffset = FVector(0, 0, 0);

	UPROPERTY(EditDefaultsOnly, Category = "Gadget")
	float GadgetMaxUpTime;
	void StartInUseTimer();

	UPROPERTY(EditDefaultsOnly, Category = "Gadget")
	float CooldownTime;
	void StartGadgetCoolDown();

	UPROPERTY(EditDefaultsOnly, Category = "Gadget")
	UInputMappingContext* GadgetMappingContext;
	UPROPERTY(EditDefaultsOnly, Category = "Gadget")
	UInputAction* ActivateAction;

	// Starts InUse timer if gadget isn't on cooldown, override to implement gadget behaviour
	virtual void Activate();
	// Starts Cooldown and automatically unequips gadget
	void FinishedUsing();

public:
	bool Equipped = false;

	void Equip();
	// Returns true if unequip succeeds
	bool Unequip();

	FVector GetSpawnOffset()
	{
		return SpawnOffset;
	}

};
