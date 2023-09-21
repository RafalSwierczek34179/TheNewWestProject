// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TheNewWestProject/Gadget/ReconGadget.h"
#include "VisionLink.generated.h"

/**
 * 
 */
UCLASS()
class THENEWWESTPROJECT_API AVisionLink : public AReconGadget
{
	GENERATED_BODY()

	virtual void Activate() override;

	virtual void FinishedUsing() override;
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	
	
	
	
};
