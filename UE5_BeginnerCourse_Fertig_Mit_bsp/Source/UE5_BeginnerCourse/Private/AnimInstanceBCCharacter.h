// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "EnumLibrary.h"
#include "AnimInstanceBCCharacter.generated.h"

/**
 * 
 */
UCLASS()
class UAnimInstanceBCCharacter : public UAnimInstance
{
	GENERATED_BODY()



public:
	UAnimInstanceBCCharacter();

	UFUNCTION(BlueprintCallable, Category = AnimationProperties)
	void UpdateAnimationProperties();

	UFUNCTION()
	virtual void NativeBeginPlay() override;

	UFUNCTION()
	void BeginTimeline( );

	UFUNCTION()
		void TimelineProgress(float _timelineAlpha);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess))
		float movementSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess))
		float sideStrenght;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess))
		float sideStrenghtDefault;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess))
		bool bIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess))
		bool bGameStarted;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess))
		class AUE5_BeginnerCourseCharacter* playerPawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess))
		UPawnMovementComponent* pawnMovement;

	UPROPERTY(EditAnywhere, Category = Timeline, meta = (AllowPrivateAccess))
		UCurveFloat* curveFloat;

	FTimeline curveTimeline;

	// Is fpr 7. Timelines and Delegates, gets changed in 8. Enums
	//bool bleanRight;

	UPROPERTY(VisibleAnywhere, Category = Timeline);
	EPlayerLeaningState playerLeaningState;

	UPROPERTY()
		UWorld* world;
};
