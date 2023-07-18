// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "ShipControlComponent.h"
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

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* CrouchAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;

	/** CallDismiss Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* CallShipAction;
	
public:
	ATheNewWestProjectCharacter();

protected:
	virtual void BeginPlay();

public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Bool for AnimBP to switch to another animation set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;

	/** Setter to set the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);

	/** Getter for the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for interaction input*/
	void Interact(const FInputActionValue& Value);

	/** Called for sprint input*/
	void StartSprint(const FInputActionValue& Value);
	void EndSprint(const FInputActionValue& Value);

	/** Called for crouch input*/
	void ToggleCrouch(const FInputActionValue& Value);

	/** Called for Calling/Dissmissing ship input*/
	void CallShip(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

private:
	/** Calls blueprint function on given object, returns true if called successfully */
	bool CallBPFunction(AActor* actorWithFunc, FString funcName);
	
//-----------------Health-------------------------------------
private:
	int health = 100;
public:
	UFUNCTION(BlueprintCallable, CAtegory="Health")
	void TakeDamage(int damage);
	
	UFUNCTION(BlueprintCallable, Category="Health")
	int GetHealth();
	
	UFUNCTION(BlueprintCallable, Category="Health")
	float CalculateDamageEffectOpacity();
	
//-----------------Crouching----------------------------------	
private:
	bool crouching;
	
//------------------Ship Controls----------------------------------------------
public:
	UFUNCTION(BlueprintCallable, Category=PlayersShip)
	bool GetIsPlayerControllingShip();

	UFUNCTION(BlueprintCallable, Category=PlayersShip)
	void SetShipControlComp(UShipControlComponent* playersShipControlComp);

	UFUNCTION(BlueprintCallable, Category=PlayersShip)
	void ToggleShipControlling();
	
private:
	// Has Move() and Look() functions for the ship
	UShipControlComponent* shipControlComp;

	bool playerControllingShip;
	
// -------------------Interacting---------------------------------------------	
private:
	// Array of actors that need to be notified when an interact event runs
	TArray<AActor*> interactEventSubscribers;
public:
	void AddToInteractEventSubscribers(AActor* newSubscriber);


};

