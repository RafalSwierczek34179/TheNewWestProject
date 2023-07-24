// Fill out your copyright notice in the Description page of Project Settings.
#include "GunSceneComp.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "kismet/GameplayStatics.h"

// Sets default values for this component's properties
UGunSceneComp::UGunSceneComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UGunSceneComp::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UGunSceneComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//------------------------------------------------------------------

void UGunSceneComp::FireGun()
{
	UAnimInstance* AnimInstance = PlayersCharacter->GetMesh1P()->GetAnimInstance();
	if (FireSound == nullptr || FireAnimation == nullptr || AnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Gun failed to fire because either the fire soun/animation on BP_Rifle GunSceneComp aren't set or failed to grab anim instance from mesh1p"));
		return;
	}
	
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, PlayersCharacter->GetActorLocation());
	AnimInstance->Montage_Play(FireAnimation, 1);	
}

void UGunSceneComp::SetupPlayerInput(ATheNewWestProjectCharacter* Player)
{
	if (Player == nullptr)
	{
		return;
	}
	PlayersCharacter = Player;

	APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());	
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
		Subsystem->AddMappingContext(FireMappingContext, 1);
	}
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
	{
		// Fire
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UGunSceneComp::FireGun);
	}
}

void UGunSceneComp::SafelyDestroyGun()
{
	APlayerController* PlayersController = Cast<APlayerController>(PlayersCharacter->GetController());
	if (PlayersController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to Destroy gun due to failed cast to players controller"));
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayersController->GetLocalPlayer());
	Subsystem->RemoveMappingContext(FireMappingContext);

	GetOwner()->Destroy();
}



