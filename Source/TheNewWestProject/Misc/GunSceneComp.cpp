// Fill out your copyright notice in the Description page of Project Settings.
#include "GunSceneComp.h"

#include "EnemyController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Camera/CameraComponent.h"
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
	if (FireSound == nullptr || FireAnimation == nullptr || AnimInstance == nullptr || FireLT_StartArrow == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Gun failed to fire because either the fire soun/animation on BP_Rifle GunSceneComp aren't set, or failed to grab anim instance from mesh1p, or FireLT_StartArrow is null"));
		return;
	}
	
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, PlayersCharacter->GetActorLocation());
	AnimInstance->Montage_Play(FireAnimation, 1);

	FHitResult OutHit;
	FVector FireLT_StartLoc = FireLT_StartArrow->GetSocketLocation(NAME_None);
	// Emergency cam forward vector in case casting to players camera component fails, less accurate as doesn't take cameras yaw into consideration
	FVector CamForwardVector = PlayersCharacter->GetActorForwardVector();
	for (UActorComponent* comp : PlayersCharacter->GetComponentsByTag(UCameraComponent::StaticClass(), FName("Camera")))
	{
		CamForwardVector = Cast<UCameraComponent>(comp)->GetForwardVector();
	}
	FVector FireLT_EndLoc = FireLT_StartLoc + (CamForwardVector * FVector(4000, 4000, 4000));
	FCollisionQueryParams CollisionQueryParameters = FCollisionQueryParams::DefaultQueryParam;
	CollisionQueryParameters.bTraceComplex = false;
	CollisionQueryParameters.AddIgnoredActor(Cast<AActor>(PlayersCharacter));

	
	if (!GetWorld()->LineTraceSingleByChannel(OutHit, FireLT_StartLoc, FireLT_EndLoc, ECC_Camera, CollisionQueryParameters) || OutHit.GetActor() == nullptr)
	{
		return;
	}
	AActor* HitActor = OutHit.GetActor();
	if (HitActor->Tags.Contains(TEXT("Enemy")))
	{
		AEnemyController* EnemyController = Cast<AEnemyController>(UAIBlueprintHelperLibrary::GetAIController(HitActor));
		if (EnemyController == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Killed enemy can't be killed as cast to their controller failed"));
			return;
		}
		EnemyController->KillSelf();
	}
}

void UGunSceneComp::SetupPlayerInput(ATheNewWestProjectCharacter* Player)
{
	if (Player == nullptr)
	{
		return;
	}
	PlayersCharacter = Player;

	for (UActorComponent* comp : PlayersCharacter->GetComponentsByTag(UArrowComponent::StaticClass(), FName("LOS")))
	{
		FireLT_StartArrow = Cast<UArrowComponent>(comp);
	}

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



