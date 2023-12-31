// Copyright Epic Games, Inc. All Rights Reserved.

#include "TheNewWestProjectCharacter.h"

#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TheNewWestProject/Misc/GunSceneComp.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Misc/OutputDeviceNull.h"
#include "Components/ActorComponent.h"
#include "Components/ArrowComponent.h"
#include "Engine/EngineTypes.h"
#include "kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"



//////////////////////////////////////////////////////////////////////////
// ATheNewWestProjectCharacter

ATheNewWestProjectCharacter::ATheNewWestProjectCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

}

void ATheNewWestProjectCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	AActor* GadgetManagerActor = GetWorld()->SpawnActor<AActor>(GadgetManagerClass, GetActorLocation(), GetActorRotation());
	GadgetManager = Cast<AGadgetManager>(GadgetManagerActor);
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	GadgetManagerActor->AttachToComponent(GetRootComponent(), AttachmentRules);
	GadgetManager->SpawnGadgets(GetRootComponent());

}

//------------------------Input--------------------------------------

void ATheNewWestProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATheNewWestProjectCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATheNewWestProjectCharacter::Look);

		//Sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ATheNewWestProjectCharacter::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ATheNewWestProjectCharacter::EndSprint);

		//Crouching
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ATheNewWestProjectCharacter::ToggleCrouch);

		//Equipping Gun
		EnhancedInputComponent->BindAction(EquipGunAction, ETriggerEvent::Started, this, &ATheNewWestProjectCharacter::ToggleGun);
		
		//Interacting
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ATheNewWestProjectCharacter::Interact);
		
		//Calling ship over
		EnhancedInputComponent->BindAction(CallShipAction, ETriggerEvent::Started, this, &ATheNewWestProjectCharacter::CallShip);

		//Displaying Bounty Information
		EnhancedInputComponent->BindAction(HUDAction, ETriggerEvent::Triggered, this, &ATheNewWestProjectCharacter::DisplayBountyUI);

		//Equipping/Unequipping gadgets
		EnhancedInputComponent->BindAction(ToggleReconGadgetAction, ETriggerEvent::Started, this, &ATheNewWestProjectCharacter::ToggleReconGadget);
		EnhancedInputComponent->BindAction(ToggleCombatGadgetAction, ETriggerEvent::Started, this, &ATheNewWestProjectCharacter::ToggleCombatGadget);

	}
}

void ATheNewWestProjectCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// Switches to ship control
	if (playerControllingShip && shipControlComp != nullptr)
	{
		shipControlComp->Move(MovementVector);
		return;
	}

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ATheNewWestProjectCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// Switches to ship control
	if (playerControllingShip && shipControlComp != nullptr)
	{
		shipControlComp->Look(LookAxisVector);
		return;
	}
	
	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X * SensitivityMultiplier);
		AddControllerPitchInput(LookAxisVector.Y * SensitivityMultiplier);
	}
}

void ATheNewWestProjectCharacter::StartSprint(const FInputActionValue& Value)
{
	this->GetCharacterMovement()->MaxWalkSpeed = 1000;
	this->GetCharacterMovement()->MaxWalkSpeedCrouched = 600;
}

void ATheNewWestProjectCharacter::EndSprint(const FInputActionValue& Value)
{
	this->GetCharacterMovement()->MaxWalkSpeed = 600;
	this->GetCharacterMovement()->MaxWalkSpeedCrouched = 300;
}

void ATheNewWestProjectCharacter::ToggleCrouch(const FInputActionValue& Value)
{
	// Crouching
	float newCapsuleHalfHeight = crouching ? 96 : 50;
	crouching = !crouching;
	Cast<UCapsuleComponent>(this->GetRootComponent())->SetCapsuleHalfHeight(newCapsuleHalfHeight);

	// Setting new location for LOS_Point so it stays within capsule collision
	FVector deltaLocation = FVector(0, 0, 20);
	deltaLocation.Z *= crouching ? -1 : 1;
	for (UActorComponent* LOS_ActorComp : GetComponentsByTag(UArrowComponent::StaticClass(), FName("LOS")))
	{
		UArrowComponent* LOS_Point = Cast<UArrowComponent>(LOS_ActorComp);
		if (LOS_Point == nullptr)
		{
			return;
		}
		FHitResult* OutSweepHitResult = new FHitResult();
		LOS_Point->AddRelativeLocation(deltaLocation, false, OutSweepHitResult, ETeleportType::None);
	}
}

void ATheNewWestProjectCharacter::ToggleGun()
{
	switch (bHasRifle)
	{
	// Un-equip gun
	case true:
		bHasRifle = false;
		// Removes Input Context for shooting and destroys actor
		for (UActorComponent* GunSceneActorComp : playersGun->GetComponentsByTag(UGunSceneComp::StaticClass(), FName("GunComp")))
		{
			Cast<UGunSceneComp>(GunSceneActorComp)->SafelyDestroyGun();
		}
		break;
	// Equip gun
	case false:
		if (GadgetManager->IsReconEquipped() && !GadgetManager->TryToUnequipRecon())
		{
			return;
		}
		if (GadgetManager->IsCombatEquipped() && !GadgetManager->TryToUnequipCombat())
		{
			return;
		}
		
		bHasRifle = true;
		// local vars needed for spawning and attaching gun to player
		FVector loc = GetActorLocation() + FVector(-50, 0, 0);
		FRotator rot = GetActorRotation();
		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		
		playersGun = GetWorld()->SpawnActor<AActor>(rifleClass, loc, rot, spawnParams);
		playersGun->AttachToComponent(Mesh1P, AttachmentRules, FName("GripPoint"));
		
		for (UActorComponent* GunSceneActorComp : playersGun->GetComponentsByTag(UGunSceneComp::StaticClass(), FName("GunComp")))
		{
			Cast<UGunSceneComp>(GunSceneActorComp)->SetupPlayerInput(this);
		}
		break;
	}
}

void ATheNewWestProjectCharacter::Interact(const FInputActionValue& Value)
{
	if (interactEventSubscribers.IsEmpty())
	{
		return;
	}
	
	const FString interactFunctionName = FString::Printf(TEXT("InteractionTriggered"));

	// Notify all subscribers of the interact event that it has been triggered
	for (AActor* subscriber : interactEventSubscribers)
	{
		if (!CallBPFunction(subscriber, interactFunctionName))
		{
			UE_LOG(LogTemp, Warning, TEXT("Interact Event subscriber failed to be notified in TheNewWestProjectCharacter.cpp Interact()"));
		}
	}
}

void ATheNewWestProjectCharacter::CallShip(const FInputActionValue& Value)
{
	AActor* ship = shipControlComp->GetOwner();
	FString CallShipFuncName = FString::Printf(TEXT("CallShip"));

	if(!CallBPFunction(ship, CallShipFuncName))
	{
		UE_LOG(LogTemp, Warning, TEXT("CallShip() function failed to notify ship in TheNewWestPlayerCharacter.cpp"))
	}
}

void ATheNewWestProjectCharacter::DisplayBountyUI_Implementation()
{
	if (ActiveBounties.IsEmpty())
	{
		WaypointLoc.Reset();
		WaypointIcon.Reset();
		WaypointDesc.Reset();
		return;
	}
	
	TArray<FVector> WaypointLocs;
	TArray<UTexture2D*> WaypointIcons;
	TArray<FString> WaypointDescs;
	UE_LOG(LogTemp, Warning, TEXT("active bounties num: %d"), ActiveBounties.Num());
	for (int i = 0; i < ActiveBounties.Num(); i++)
	{
		float Dist = FVector::Distance(GetActorLocation(), ActiveBounties[i]->GetStepLoc());
		UE_LOG(LogTemp, Warning, TEXT("dist to bounty: %f"), Dist);
		
		if ((i > 0) && (ActiveBounties[i] == nullptr || Dist >= 13000))
		{
			continue;
		}

		WaypointLocs.Add(ActiveBounties[i]->GetActiveWaypointLoc());
		WaypointIcons.Add(ActiveBounties[i]->GetActiveWaypointIcon());
		WaypointDescs.Add(ActiveBounties[i]->GetActiveStepDesc());
	}

	WaypointLoc = WaypointLocs;
	WaypointIcon = WaypointIcons;
	WaypointDesc = WaypointDescs;
	
}

void ATheNewWestProjectCharacter::ToggleReconGadget()
{
	if (bHasRifle)
	{
		ToggleGun();
	}

	if (GadgetManager->IsReconEquipped())
	{
		GadgetManager->TryToUnequipRecon();
		return;
	}

	if (GadgetManager->IsCombatEquipped() && !GadgetManager->TryToUnequipCombat())
	{
		// Do nothing if combat is equipped and can't be unequipped at this moment
		return;
	}

	GadgetManager->EquipRecon();
}

void ATheNewWestProjectCharacter::ToggleCombatGadget()
{
	if (bHasRifle)
	{
		ToggleGun();
	}

	if (GadgetManager->IsCombatEquipped())
	{
		GadgetManager->TryToUnequipCombat();
		return;
	}

	if (GadgetManager->IsReconEquipped() && !GadgetManager->TryToUnequipRecon())
	{
		// Do nothing if recon is equipped and can't be unequipped at the moment
		return;
	}

	GadgetManager->EquipCombat();
}




//------------------------Health--------------------------------------

void ATheNewWestProjectCharacter::TakeDamage(int damage)
{
	health -= damage;

	if (health > 0)
	{
		return;
	}
	UGameplayStatics::OpenLevel(GetWorld(), FName("ShantyTown_Map"));
}

int ATheNewWestProjectCharacter::GetHealth()
{
	return health;
}

float ATheNewWestProjectCharacter::CalculateDamageEffectOpacity()
{
	return (float)(100-health)/100;
}

//-----------------Players Ship------------------------------------------
 bool ATheNewWestProjectCharacter::GetIsPlayerControllingShip()
 {
 	return playerControllingShip;
 }
 
 void ATheNewWestProjectCharacter::SetShipControlComp(UShipControlComponent* playersShipControlComp)
 {
 	shipControlComp = playersShipControlComp;
 }
 
 void ATheNewWestProjectCharacter::ToggleShipControlling()
 {
 	playerControllingShip = !playerControllingShip;

	// Ship HUD
	
	if (!playerControllingShip)
	{
		DespawnShipHUD();
		return;
	}

	TArray<FString> StepDescs;
	TArray<UTexture2D*> WaypointIcons;
	TArray<FVector> WaypointLocs;

	for (ABountyClass* BC : ActiveBounties)
	{
		StepDescs.Add(BC->GetActiveStepDesc());
		WaypointIcons.Add(BC->GetActiveWaypointIcon());
		WaypointLocs.Add(BC->GetActiveWaypointLoc());
	}
	SpawnShipHUD(StepDescs, WaypointIcons, WaypointLocs);
 }

//------------------------Misc--------------------------------------

void ATheNewWestProjectCharacter::AddToInteractEventSubscribers(AActor* newSubscriber)
{
	interactEventSubscribers.Add(newSubscriber);
}

bool ATheNewWestProjectCharacter::CallBPFunction(AActor* actorWithFunc, FString funcName)
{
	FOutputDeviceNull nullOutDevice;
	return actorWithFunc->CallFunctionByNameWithArguments(*funcName, nullOutDevice, NULL, true);
}





