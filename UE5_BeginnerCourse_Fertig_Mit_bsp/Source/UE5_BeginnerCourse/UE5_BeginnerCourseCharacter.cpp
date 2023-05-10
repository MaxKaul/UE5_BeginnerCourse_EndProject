// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE5_BeginnerCourseCharacter.h"

#include "BCPlayerStart.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InteractableBase.h"
#include "PlayerInteractor.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"


//////////////////////////////////////////////////////////////////////////
// AUE5_BeginnerCourseCharacter

AUE5_BeginnerCourseCharacter::AUE5_BeginnerCourseCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); 

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; 
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); 
	FollowCamera->bUsePawnControlRotation = false;

	playerInteractor = CreateDefaultSubobject<UPlayerInteractor>("PlayerInteractor");

	health = 100.f;
}

void AUE5_BeginnerCourseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (playerSpawnSubclass)
		playerSpawn = Cast<ABCPlayerStart>(UGameplayStatics::GetActorOfClass(GetWorld(), playerSpawnSubclass));

	playerID = UGameplayStatics::GetPlayerControllerID(Cast<APlayerController>(Controller));
}

void AUE5_BeginnerCourseCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUE5_BeginnerCourseCharacter::Move);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUE5_BeginnerCourseCharacter::Look);
		EnhancedInputComponent->BindAction(LookActionEnd, ETriggerEvent::Completed, this, &AUE5_BeginnerCourseCharacter::LookEnd);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AUE5_BeginnerCourseCharacter::Interact);
	}

}

void AUE5_BeginnerCourseCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AUE5_BeginnerCourseCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);

		currYawInput = LookAxisVector.X;
	}
}

void AUE5_BeginnerCourseCharacter::LookEnd(const FInputActionValue& Value)
{
	currYawInput = 0.f;
}

void AUE5_BeginnerCourseCharacter::Interact()
{
	if (currInteractObject)
		currInteractObject->OnPlayerInteraction(playerID);
}

void AUE5_BeginnerCourseCharacter::TakeDamage(float _damage)
{
	float currhp = health;

	if (currhp - _damage <= 0)
		PlayDeath();
	else
		health -= _damage;
}

void AUE5_BeginnerCourseCharacter::PlayDeath()
{
	if(!playerSpawn)
	{
		UE_LOG(LogTemp,Warning,TEXT("AUE5_BeginnerCourseCharacter, !playerSpawn"))
		return;
	}

	bHasDied = true;

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AUE5_BeginnerCourseCharacter::CleanAfterDeath, playerSpawn->GetRespawnTime(), false);

	GetController()->UnPossess();
	GetMesh()->SetSimulatePhysics(true);
}

void AUE5_BeginnerCourseCharacter::CleanAfterDeath()
{
	UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
	playerSpawn->RespawnPlayer(this);
}
