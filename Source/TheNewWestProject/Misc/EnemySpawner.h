// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "EnemySpawner.generated.h"

UCLASS()
class THENEWWESTPROJECT_API AEnemySpawner : public AActor
{
	GENERATED_BODY()

//-----------------Defaults------------------
public:	
	AEnemySpawner();
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;
	
//----------------------------------------
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy Spawner")
	TSubclassOf<ACharacter> enemyCharacterClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy Spawner")
	TArray<FTransform> spawnTransforms;
	

};
