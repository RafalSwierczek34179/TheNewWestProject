// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CustomMathUtility.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THENEWWESTPROJECT_API UCustomMathUtility : public UActorComponent
{
	GENERATED_BODY()
public:	
	UCustomMathUtility();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	virtual void BeginPlay() override;

public:
	// linear scaling, scale = 1 when distance is <= 4000, and 10 when >= 30,000
	UFUNCTION(BlueprintCallable, Category = "Graphing")
	float WaypointScalingCalculator(float Distance);


		
};
