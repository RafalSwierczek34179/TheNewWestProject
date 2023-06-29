// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActorComponent.h"

#include "Kismet/GameplayStatics.h"
#include "TheNewWestProjectCharacter.h"



// Sets default values for this component's properties
UInteractableActorComponent::UInteractableActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractableActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// Subscribes the actor which this component sits on to the players interact event
	ATheNewWestProjectCharacter* playersCharClass = Cast<ATheNewWestProjectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	playersCharClass->AddToInteractEventSubscribers(GetOwner());
	
}


// Called every frame
void UInteractableActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}



