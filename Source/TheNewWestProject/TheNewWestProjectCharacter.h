// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gadget/GadgetManager.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "TheNewWestProject/Misc/ShipControlComponent.h"
#include "InputAction.h"
#include "TheNewWestProject/BountySystem/BountyClass.h"
#include "TheNewWestProjectCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

UCLASS(config=Game)
class ATheNewWestProjectCharacter : public ACharacter
{
	GENERATED_BODY()
	
	// 	------------------------------Properties-----------------------------------------------
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
    USkeletalMeshComponent* Mesh1P;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(EditAnywhere, Category=Gun)
	TSubclassOf<AActor> rifleClass;

	AActor* playersGun;

	int health = 100;

	bool crouching;

	UPROPERTY(EditDefaultsOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float SensitivityMultiplier = 1;

	// Array of actors that need to be notified when an interact event runs
	TArray<AActor*> interactEventSubscribers;

	// Has Move() and Look() functions for the ship
	UShipControlComponent* shipControlComp;
                                                                                                              
	bool playerControllingShip;

	AGadgetManager* GadgetManager;
	
	// ------------------------------Functions-----------------------------------------------
	/** Calls blueprint function on given object, returns true if called successfully */
	bool CallBPFunction(AActor* actorWithFunc, FString funcName);
	

protected:
	// 	------------------------------Properties-----------------------------------------------
	UPROPERTY(EditDefaultsOnly, Category = "Gadget")
	TSubclassOf<AGadgetManager> GadgetManagerClass;
	
	// ------------------------------Functions-----------------------------------------------
	virtual void BeginPlay();

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Waypoint")
	void SpawnShipHUD(const TArray<FString> &StepDescs, const TArray<UTexture2D*> &WaypointIcons, const TArray<FVector> &WaypointLocs);

	UFUNCTION(BlueprintImplementableEvent, Category = "Waypoint")
	void DespawnShipHUD();
public:
	ATheNewWestProjectCharacter();

	// 	------------------------------Properties-----------------------------------------------
	/** Bool for AnimBP to switch to another animation set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;

	TArray<ABountyClass*> ActiveBounties;

	UPROPERTY(BlueprintReadOnly, Category = "Bounty")
	TArray<FVector> WaypointLoc;

	UPROPERTY(BlueprintReadOnly, Category = "Bounty")
	TArray<UTexture2D*> WaypointIcon;

	UPROPERTY(BlueprintReadOnly, Category = "Bounty")
	TArray<FString> WaypointDesc;	

	// ------------------------------Functions-----------------------------------------------
	UFUNCTION(BlueprintCallable, CAtegory="Health")
	void TakeDamage(int damage);

	UFUNCTION(BlueprintCallable, Category="Health")
    int GetHealth();

	UFUNCTION(BlueprintCallable, Category="Health")
	float CalculateDamageEffectOpacity();

	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }

	void AddToInteractEventSubscribers(AActor* newSubscriber);

	UFUNCTION(BlueprintCallable, Category=PlayersShip)
	bool GetIsPlayerControllingShip();
                                                              
	UFUNCTION(BlueprintCallable, Category=PlayersShip)
	void SetShipControlComp(UShipControlComponent* playersShipControlComp);
                                                              
	UFUNCTION(BlueprintCallable, Category=PlayersShip)
	void ToggleShipControlling();
	
//------------------------------------Input---------------------------------------
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;
	
	// Input Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	                                   
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SprintAction;
                                   
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;
                                   
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* EquipGunAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* InteractAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* CallShipAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* HUDAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* ToggleReconGadgetAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* ToggleCombatGadgetAction;

protected:
	// Input Methods, jump method already exists and is run from ACharacter
	void Look(const FInputActionValue& Value);

	void Move(const FInputActionValue& Value);

	void StartSprint(const FInputActionValue& Value);
	
	void EndSprint(const FInputActionValue& Value);

	void ToggleCrouch(const FInputActionValue& Value);

	void ToggleGun();

	void Interact(const FInputActionValue& Value);

	void CallShip(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, Category = "Player Character")
	void DisplayBountyUI();

	void ToggleReconGadget();

	void ToggleCombatGadget();
};

