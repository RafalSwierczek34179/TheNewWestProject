// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	// FActorSpawnParameters spawnParams;
	// for (FTransform spawnTransform : spawnTransforms)
	// {
	// 	GetWorld()->SpawnActor<AActor>(enemyCharacterClass, spawnTransform.GetLocation(), spawnTransform.GetRotation().Rotator(), spawnParams);
	// }
	
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

