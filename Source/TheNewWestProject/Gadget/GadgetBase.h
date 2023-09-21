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
	
	FTimerHandle CooldownHandle;
	void StartGadgetCoolDown();
	bool GadgetOnCooldown = false;
	
	FTimerHandle InUseHandle;
	void StartInUseTimer();
    bool GadgetInUse = false;

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

	bool GadgetCantBeActivated()
	{
		return GadgetOnCooldown || GadgetInUse;
	}
	
	UPROPERTY(EditDefaultsOnly, Category = "Gadget")
	float GadgetMaxUpTime;
	
	UPROPERTY(EditDefaultsOnly, Category = "Gadget")
	float CooldownTime;
	

	UPROPERTY(EditDefaultsOnly, Category = "Gadget")
	UInputMappingContext* GadgetMappingContext;
	UPROPERTY(EditDefaultsOnly, Category = "Gadget")
	UInputAction* ActivateAction;

	// Used by child classes to run custom gadget behaviour
	virtual void Activate();

	// Starts Cooldown and automatically unequips gadget, used by child classes to implement custom behaviour
	virtual void FinishedUsing();

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
