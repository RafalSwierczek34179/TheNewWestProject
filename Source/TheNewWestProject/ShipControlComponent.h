// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/ArrowComponent.h"
#include "ShipControlComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THENEWWESTPROJECT_API UShipControlComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UShipControlComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	// Overrides players move method to control the ship instead, called by players character 
	void Move(FVector2D MovementVector);
	// Overrides players look method to control the ship instead, called by players character
	void Look(FVector2D LookAxisVector);

private:
	UFUNCTION(BlueprintCallable, Category=PlayersShip)
	void SetPlayer(AActor* player);
	AActor* playersActor;
	APawn* playersPawn;

	// Used to find current seat location in world space
	UFUNCTION(BlueprintCallable, Category=PlayersShip)
	void SetSeatAnchor(UArrowComponent* seatAnchorArrowComp);
	UArrowComponent* seatAnchor;
	
	// Anchors player to their seat in the ship when controlling it
	void UpdatePlayerLocation();
	void UpdatePlayerRotation();

	UFUNCTION(BlueprintCallable, Category=PlayersShip)
	void SetPlayerControllingShip(bool isPlayerControllingShip);
	bool playerControllingShip;

	
	
		
};
