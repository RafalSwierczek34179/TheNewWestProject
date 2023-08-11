// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "TheNewWestProjectProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TheNewWestProject/TheNewWestProjectCharacter.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}

void UTP_WeaponComponent::Fire()
{
	if (playersCharacter == nullptr || playersCharacter->GetController() == nullptr)
	{
		return;
	}

	// Try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(playersCharacter->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
	
			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
			// Spawn the projectile at the muzzle
			World->SpawnActor<ATheNewWestProjectProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}
	
	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, playersCharacter->GetActorLocation());
	}
	
	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		//UAnimInstance* AnimInstance = playersCharacter->GetMesh1P()->GetAnimInstance();
		// if (AnimInstance != nullptr)
		// {
		// 	AnimInstance->Montage_Play(FireAnimation, 1.f);
		// }
	}
}

void UTP_WeaponComponent::SetupGunInput(APlayerController* PlayerController)
{

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
		Subsystem->AddMappingContext(FireMappingContext, 1);
	}
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
	{
		// Fire
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Fire);
	}
		
}


// void UTP_WeaponComponent::AttachWeapon(USceneComponent* arms)
// {
// 	// Attach the weapon to the First Person Character
// 	//UE_LOG(LogTemp, Warning, TEXT("Attachment worked? %s"), GetOwner()->AttachToComponent(TargetCharacter->GetMesh1P(), AttachmentRules, FName("GripPoint")) ? TEXT("true") : TEXT("false"));
// 	//GetOwner()->AttachToComponent(TargetCharacter->GetMesh1P(), AttachmentRules, FName("GripPoint"));
//
// 	//switch bHasRifle so the animation blueprint can switch to another animation set
// 	// TargetCharacter->SetHasRifle(true);
// 	//
// 	// // Set up action bindings
// 	// if (APlayerController* PlayerController = Cast<APlayerController>(TargetCharacter->GetController()))
// 	// {
// 	// 	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
// 	// 	{
// 	// 		// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
// 	// 		Subsystem->AddMappingContext(FireMappingContext, 1);
// 	// 	}
// 	//
// 	// 	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
// 	// 	{
// 	// 		// Fire
// 	// 		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Fire);
// 	// 	}
// 	// }
// }

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (playersCharacter == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(playersCharacter->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}