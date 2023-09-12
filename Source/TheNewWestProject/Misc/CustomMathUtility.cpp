// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomMathUtility.h"

UCustomMathUtility::UCustomMathUtility()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCustomMathUtility::BeginPlay()
{
	Super::BeginPlay();
}

void UCustomMathUtility::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

float UCustomMathUtility::WaypointScalingCalculator(float Distance)
{
	if (Distance <= 200)
	{
		return 0.2;
	}
	if (Distance <= 2000)
	{
		return ((Distance / 2250.0) + (1.0 / 9.0));
	} 
	
	if (Distance <= 4000)
	{		
		return 1;
	}
	if (Distance >= 30000)
	{
		return 10;
	}
	
	// Good luck o7
	return ((Distance * 9) / 26000.0) - (5.0 / 13.0);
}
