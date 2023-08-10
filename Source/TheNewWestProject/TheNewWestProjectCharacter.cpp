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
		EnhancedInputComponent->BindAction(EquipGunAction, ETriggerEvent::Started, this, &ATheNewWestProjectCharacter::EquipGun);
		
		//Interacting
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ATheNewWestProjectCharacter::Interact);
		
		//Calling ship over
		EnhancedInputComponent->BindAction(CallShipAction, ETriggerEvent::Started, this, &ATheNewWestProjectCharacter::CallShip);
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

void ATheNewWestProjectCharacter::EquipGun(const FInputActionValue& Value)
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





