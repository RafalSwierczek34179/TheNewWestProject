// Fill out your copyright notice in the Description page of Project Settings.


#include "GadgetBase.h"

// Sets default values
AGadgetBase::AGadgetBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGadgetBase::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = GetWorld()->GetFirstPlayerController();
	
}

// Called every frame
void AGadgetBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGadgetBase::Equip()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AActor* GadgetActor = GetWorld()->SpawnActor<AActor>(GadgetClass, GetActorLocation(), GetActorRotation(), SpawnParams);
	GadgetBP = Cast<AGadgetBP>(GadgetActor);

	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	GadgetActor->AttachToComponent(GetRootComponent(), AttachmentRules);

	Equipped = true;

	
}

bool AGadgetBase::Unequip()
{
	return false;
}

