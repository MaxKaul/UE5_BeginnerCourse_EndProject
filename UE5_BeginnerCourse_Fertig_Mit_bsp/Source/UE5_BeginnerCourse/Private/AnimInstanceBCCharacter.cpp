// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstanceBCCharacter.h"

#include "GameFramework/PawnMovementComponent.h"
#include "UE5_BeginnerCourse/UE5_BeginnerCourseCharacter.h"

UAnimInstanceBCCharacter::UAnimInstanceBCCharacter()
{
	bGameStarted = false;
	bIsInAir = false;

	sideStrenghtDefault = 50.f;
	sideStrenght = sideStrenghtDefault;
	movementSpeed = 0.f;
}

void UAnimInstanceBCCharacter::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	playerPawn = Cast<AUE5_BeginnerCourseCharacter>(TryGetPawnOwner());

	world = GetWorld();
}

void UAnimInstanceBCCharacter::UpdateAnimationProperties()
{
	if (playerPawn)
		pawnMovement = playerPawn->GetMovementComponent();
	else if (!playerPawn || !pawnMovement)
		return;

	FVector speed = playerPawn->GetVelocity();
	FVector lateralSpeed = FVector(speed.X, speed.Y, 0);

	movementSpeed = lateralSpeed.Size();

	bIsInAir = pawnMovement->IsFalling();

	if(movementSpeed <= 0)
	{
		sideStrenght = sideStrenghtDefault;
		return;
	}

	// --------------------------------------------------
	// Animation to Side without Lerp and Timeline 
	// To be Implemented in Point 6. AnimInstance and Changed in 7. Timelines and Curves
	//
	// 	if (playerPawn->GetCurrYawInput() > 0)
	//			sideStrenght = sideStrenghtDefault * 2;
	//		else if (playerPawn->GetCurrYawInput() < 0)
	//			sideStrenght = 0;
	//
	// --------------------------------------------------


	// --------------------------------------------------
	// 	// Animation to Side wit Lerp and Timeline 
	//     To be Implemented in Point 7. Timeline and Curves
	// --------------------------------------------------

	if (!curveFloat)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAnimInstanceBCCharacter, !curveFloat"));
		return;
	}

	if (playerPawn->GetCurrYawInput() > 0)
		playerLeaningState = EPlayerLeaningState::PLS_LeanRight;
	else if (playerPawn->GetCurrYawInput() < 0)
		playerLeaningState = EPlayerLeaningState::PLS_LeanLeft;

	if(!world)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAnimInstanceBCCharacter, !world"));
		return;
	}

	BeginTimeline();
	curveTimeline.TickTimeline(world->GetDeltaSeconds());
}

void UAnimInstanceBCCharacter::BeginTimeline()
{
	if(curveTimeline.IsPlaying())
		return;

	FOnTimelineFloat timelineprogress;

	timelineprogress.BindUFunction(this, FName("TimelineProgress"));

	curveTimeline.SetTimelineLengthMode(TL_LastKeyFrame);
	curveTimeline.SetLooping(false);

	curveTimeline.AddInterpFloat(curveFloat, timelineprogress);

	curveTimeline.PlayFromStart();
}

void UAnimInstanceBCCharacter::TimelineProgress(float _timelineAlpha)
{
	switch (playerLeaningState)
	{
	case EPlayerLeaningState::PLS_LeanRight:
		if(sideStrenght < 99.f)
			sideStrenght = FMath::Lerp(sideStrenght, sideStrenghtDefault * 2, _timelineAlpha / 100);
		break;

	case EPlayerLeaningState::PLS_LeanLeft:
		if(sideStrenght > 1.f)
			sideStrenght = FMath::Lerp(sideStrenght, 0, _timelineAlpha / 100);
		break;

	case EPlayerLeaningState::PLS_Default:
		sideStrenght = sideStrenghtDefault;
		break;

	default:
		sideStrenght = sideStrenghtDefault;
		break;
	}
}
