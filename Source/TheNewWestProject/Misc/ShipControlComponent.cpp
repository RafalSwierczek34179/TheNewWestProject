// Fill out your copyright notice in the Description page of Project Settings.


#include "ShipControlComponent.h"

// Sets default values for this component's properties
UShipControlComponent::UShipControlComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UShipControlComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UShipControlComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (playerControllingShip && playersPawn != nullptr)
	{
		UpdatePlayerLocation();
		UpdatePlayerRotation();
	}
}

//----------------------Ship Controls-----------------------------------

void UShipControlComponent::Move(FVector2D MovementVector)
{
	// Get ships direction vectors, and ignore their pitch
	FVector shipForwardVector = GetOwner()->GetActorForwardVector();
	shipForwardVector.Z = 0;
	FVector shipRightVector = GetOwner()->GetActorRightVector();
	shipRightVector.Y += shipRightVector.Z;
	shipRightVector.Z = 0;
	
	APawn* shipsPawn = Cast<APawn>(GetOwner());
	if (shipsPawn != nullptr)
	{
		shipsPawn->AddMovementInput(shipForwardVector, MovementVector.Y);
		shipsPawn->AddMovementInput(shipRightVector, MovementVector.X);
	}
}

void UShipControlComponent::UpdatePlayerLocation()
{
	playersActor->SetActorLocation(seatAnchor->GetComponentLocation());
}

void UShipControlComponent::Look(FVector2D LookAxisVector)
{
	FRotator shipRotator = GetOwner()->GetActorRotation();

	shipRotator.Yaw += LookAxisVector.X;
	shipRotator.Pitch -= LookAxisVector.Y;

	shipRotator.Pitch = shipRotator.Pitch < -30 ? -30 : shipRotator.Pitch;
	shipRotator.Pitch = shipRotator.Pitch > 30 ? 30 : shipRotator.Pitch;
	
	GetOwner()->SetActorRotation(shipRotator);
}

void UShipControlComponent::UpdatePlayerRotation()
{
	playersPawn->Controller->SetControlRotation(GetOwner()->GetActorRotation());
}

void UShipControlComponent::SetPlayer(AActor* player)
{
	playersActor = player;
	playersPawn = Cast<APawn>(player);
}

void UShipControlComponent::SetSeatAnchor(UArrowComponent* seatAnchorArrowComp)
{
	seatAnchor = seatAnchorArrowComp;
}

void UShipControlComponent::SetPlayerControllingShip(bool isPlayerControllingShip)
{
	playerControllingShip = isPlayerControllingShip;
}



