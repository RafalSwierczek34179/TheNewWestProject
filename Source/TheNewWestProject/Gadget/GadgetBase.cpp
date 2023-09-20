// Fill out your copyright notice in the Description page of Project Settings.


#include "GadgetBase.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

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
	GadgetVisual = Cast<AGadgetBP>(GadgetActor);

	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	GadgetActor->AttachToComponent(GetRootComponent(), AttachmentRules);

	Equipped = true;
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	Subsystem->AddMappingContext(GadgetMappingContext, 2);
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
	EnhancedInputComponent->BindAction(ActivateAction, ETriggerEvent::Started, this, &AGadgetBase::Activate);
}

bool AGadgetBase::Unequip()
{
	if (GadgetInUse)
	{
		return false;
	}

	GadgetVisual->Destroy();
	Equipped = false;

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	Subsystem->RemoveMappingContext(GadgetMappingContext);
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
	EnhancedInputComponent->ClearActionBindings();
	
	return true;
}

void AGadgetBase::Activate()
{
	if (!GadgetInUse && !GadgetOnCooldown)
	{
		UE_LOG(LogTemp, Warning, TEXT("Gadget has been used"));
		StartInUseTimer();
	}
	
	// The rest is implemented in the inherited function
}

void AGadgetBase::FinishedUsing()
{
	UE_LOG(LogTemp, Warning, TEXT("Gadget is no longer being used"));
	
	GadgetInUse = false;
 	InUseHandle.Invalidate();
	
	StartGadgetCoolDown();
	Unequip();
}

// ----------------- Timers -----------------

void AGadgetBase::StartGadgetCoolDown()
{
	UE_LOG(LogTemp, Warning, TEXT("Gadget is now cooling down"));
	
	GadgetOnCooldown = true;
	GetWorld()->GetTimerManager().SetTimer(CooldownHandle, this, &AGadgetBase::ReadyGadget, CooldownTime, false);
}

void AGadgetBase::StartInUseTimer()
{
	UE_LOG(LogTemp, Warning, TEXT("Gadget In Use Timer Started"));
	
	GadgetInUse = true;
	GetWorld()->GetTimerManager().SetTimer(InUseHandle, this, &AGadgetBase::FinishedUsing, GadgetMaxUpTime, false);
}



