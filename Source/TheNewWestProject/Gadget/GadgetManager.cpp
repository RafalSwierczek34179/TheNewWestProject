// Fill out your copyright notice in the Description page of Project Settings.


#include "GadgetManager.h"

// Sets default values
AGadgetManager::AGadgetManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGadgetManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGadgetManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGadgetManager::SpawnGadgets(USceneComponent* PlayersRootComp)
{
	if (ReconClass == nullptr || CombatClass == nullptr)
	{
		UE_LOG(LogTemp, Fatal, TEXT("Recon or Combat class hasn't been set"));
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ReconClass, GetActorLocation(), GetActorRotation(), SpawnParams);
	ReconGadget = Cast<AReconGadget>(SpawnedActor);
	SpawnedActor->AttachToComponent(PlayersRootComp, AttachmentRules);
	SpawnedActor->SetActorRelativeLocation(ReconGadget->GetSpawnOffset());

	SpawnedActor = GetWorld()->SpawnActor<AActor>(CombatClass, GetActorLocation(), GetActorRotation(), SpawnParams);
	CombatGadget = Cast<ACombatGadget>(SpawnedActor);
	SpawnedActor->AttachToComponent(PlayersRootComp, AttachmentRules);
	SpawnedActor->SetActorRelativeLocation(CombatGadget->GetSpawnOffset());
}





