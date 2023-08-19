// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StepClass.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCompletedStep);

UCLASS()
class THENEWWESTPROJECT_API AStepClass : public AActor
{
	GENERATED_BODY()
public:	
	AStepClass();
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;

// ----------------------------------

private:
	UFUNCTION(BlueprintCallable, Category = "Step")
	void BroadcastCompletion();

	UPROPERTY(EditDefaultsOnly, Category = "Step")
	FString Description;
	UPROPERTY(EditDefaultsOnly, Category = "Step")
	FVector WaypointLoc;
	UPROPERTY(EditDefaultsOnly, Category = "Step")
	UTexture2D* WaypointIcon;
	UPROPERTY(EditDefaultsOnly, Category = "Step")
	FTransform StepSpawnTransform;

public:
	FCompletedStep CompletedStepDelegate;
	
	UPROPERTY(VisibleAnywhere, Category = "Step")
	bool Active = false;

	// Getters for Step Properties
	FString GetDescription()
	{
		return Description;
	};
	FVector GetWaypointLoc()
	{
		return WaypointLoc;
	}
	UTexture2D* GetWaypointIcon()
	{
		return WaypointIcon;
	}
	FTransform GetStepSpawnTransform()
	{
		return StepSpawnTransform;
	}
	
	

	

};
