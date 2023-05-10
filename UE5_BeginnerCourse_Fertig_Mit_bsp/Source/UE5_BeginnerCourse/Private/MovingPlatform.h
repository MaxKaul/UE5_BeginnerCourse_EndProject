// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConditionLibrary.h"
#include "InteractableBase.h"
#include "Components/TimelineComponent.h"
#include "Engine/DataTable.h"
#include "MovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class AMovingPlatform : public AInteractableBase
{
	GENERATED_BODY()
	
public:
	AMovingPlatform();

protected:
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

	virtual bool PlayerInteraction_OnOverlapBegin(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult) override;
	virtual bool PlayerInteraction_OnOverlapEnd(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex) override;

	virtual bool ConditionCheck() override;

	virtual void InitConditions() override;
	virtual void ResetConditions() override;
	virtual void ConditionCallback(bool _status,int _objectID) override;

	UPROPERTY(EditAnywhere, Category = Timeline, meta = (AllowPrivateAccess))
		float completionTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess))
		class ASplinePath* pathMovingSpline;

	UPROPERTY(EditAnywhere, Category = Timeline, meta = (AllowPrivateAccess))
		UCurveFloat* curveFloat;

	// Vor Data Table
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Conditions, meta = (AllowPrivateAccess))
	//	bool bConditional_StartEnd;
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Conditions, meta = (AllowPrivateAccess))
	//	bool bConditional_EndStart;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Conditions, meta = (AllowPrivateAccess))
		bool bConditionalActivation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Conditions, meta = (AllowPrivateAccess))
		bool bConditionsMet;

	FTimeline curveTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Conditions, meta = (AllowPrivateAccess))
		TArray<class AInteractableButton*> conditionButtons;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Conditions, meta = (AllowPrivateAccess))
		UDataTable* conditionTable;

	UPROPERTY(VisibleAnywhere, Category = Conditions)
	FConditionRowBase activationCondition;

	UPROPERTY(VisibleAnywhere, Category = Conditions)
	TArray<int> activationCombination_Fill;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = Conditions, meta = (AllowPrivateAccess))
	FName rowContentID;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = Conditions, meta = (AllowPrivateAccess))
	int mapContentID;

	UPROPERTY()
		class AUE5_BeginnerCourseCharacter* currOwner;

	UPROPERTY()
	bool bActivationConditionMet;

	UPROPERTY()
	bool bHasReachedEnd;

	UPROPERTY()
	float lerpDistance;

	UFUNCTION()
		void BeginTimeline();

	UFUNCTION()
		void TimelineProgress(float _timelineAlpha);
};