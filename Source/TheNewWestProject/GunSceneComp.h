// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TheNewWestProjectCharacter.h"
#include "Components/SceneComponent.h"
#include "GunSceneComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THENEWWESTPROJECT_API UGunSceneComp : public USceneComponent
{
	GENERATED_BODY()
	
// Boiler plate
public:	
	UGunSceneComp();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	virtual void BeginPlay() override;

//-------------------------------------------------------------
public:
	void FireGun();
	void SetupPlayerInput(ATheNewWestProjectCharacter* Player);
	void SafelyDestroyGun();

private:
	ATheNewWestProjectCharacter* PlayersCharacter;

	// Shooting properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* FireMappingContext;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
    UInputAction* FireAction;
	UPROPERTY(EditAnywhere, Category = Gameplay)
	UAnimMontage* FireAnimation;
	UPROPERTY(EditAnywhere, Category=Gameplay)
	USoundBase* FireSound;
		
};
