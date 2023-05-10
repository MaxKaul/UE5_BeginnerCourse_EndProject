// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

#include "InteractableButton.h"
#include "SplinePath.h"
#include "Components/SplineComponent.h"
#include "Engine/DataTable.h"
#include "Kismet/KismetMathLibrary.h"
#include "UE5_BeginnerCourse/UE5_BeginnerCourseCharacter.h"

AMovingPlatform::AMovingPlatform()
{
	completionTime = 10.f;
	//bConditional_StartEnd = false;
	//bConditional_EndStart = false;
	bActivationConditionMet = false;
	bConditionalActivation = false;
	rowContentID = "0";
	mapContentID = 0;
}

void AMovingPlatform::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (curveTimeline.IsPlaying())
		curveTimeline.TickTimeline(DeltaSeconds);
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	if (conditionTable)
		InitConditions();
	else
		UE_LOG(LogTemp, Warning, TEXT("AMovingPlatform, !conditionTable"));
}

bool AMovingPlatform::PlayerInteraction_OnOverlapBegin(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult)
{
	bool status = false;

	if (Super::PlayerInteraction_OnOverlapBegin(_overlappedComponent, _otherActor, _otherComp, _otherBodyIndex, _bFromSweep, _sweepResult))
	{
		if (_otherActor /*&&  Cast<AUE5_BeginnerCourseCharacter>(_otherActor)*/ && !curveTimeline.IsPlaying() && ConditionCheck())
		{
			if(AUE5_BeginnerCourseCharacter* player = Cast<AUE5_BeginnerCourseCharacter>(_otherActor))
			{
				currOwner = player;
				BeginTimeline();
				//status = true;
			}

		}
	}

	return status;
}

bool AMovingPlatform::PlayerInteraction_OnOverlapEnd(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex)
{

	if (_otherActor && !curveTimeline.IsPlaying() && ConditionCheck())
	{
		if (AUE5_BeginnerCourseCharacter* player = Cast<AUE5_BeginnerCourseCharacter>(_otherActor))
			currOwner = nullptr;
	}

	return Super::PlayerInteraction_OnOverlapEnd(_overlappedComponent, _otherActor, _otherComp, _otherBodyIndex);
}

void AMovingPlatform::BeginTimeline()
{
	FOnTimelineFloat timelineprogress;

	timelineprogress.BindUFunction(this, FName("TimelineProgress"));
	
	curveTimeline.SetTimelineLengthMode(TL_LastKeyFrame);
	curveTimeline.SetPlayRate(1.f / completionTime);
	curveTimeline.SetLooping(false);

	curveTimeline.AddInterpFloat(curveFloat, timelineprogress);

	if (FMath::IsNearlyEqual(lerpDistance, pathMovingSpline->GetSplinePath()->GetSplineLength()))
		bHasReachedEnd = true;
	else if (FMath::IsNearlyEqual(lerpDistance, 0))
		bHasReachedEnd = false;

	curveTimeline.PlayFromStart();
}

void AMovingPlatform::TimelineProgress(float _timelineAlpha)
{
	if(!pathMovingSpline)
	{
		UE_LOG(LogTemp, Warning, TEXT("AMovingPlatform,!pathMovingSpline"));
		return;
	}

	if(!bHasReachedEnd)
	{
		lerpDistance = UKismetMathLibrary::Lerp(0, pathMovingSpline->GetSplinePath()->GetSplineLength(), _timelineAlpha);
		SetActorLocation(pathMovingSpline->GetSplinePath()->GetLocationAtDistanceAlongSpline(lerpDistance, ESplineCoordinateSpace::World));
	}
	else if(bHasReachedEnd)
	{
		lerpDistance = UKismetMathLibrary::Lerp(pathMovingSpline->GetSplinePath()->GetSplineLength(), 0, _timelineAlpha);
		SetActorLocation(pathMovingSpline->GetSplinePath()->GetLocationAtDistanceAlongSpline(lerpDistance, ESplineCoordinateSpace::World));
	}
}

bool AMovingPlatform::ConditionCheck()
{
	bool status = true;

	// -> Alt bevor Real Conditions
	//if (bConditional_StartEnd && !bHasReachedEnd)
	//{
	//	if (ConditionsAreMet())
	//		status = true;
	//	else
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("CONDITIONS NOT MET"));
	//		status = false;
	//	}
	//}
	//else if (bConditional_EndStart && bHasReachedEnd)
	//{
	//	if (ConditionsAreMet())
	//		status = true;
	//	else
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("CONDITIONS NOT MET"));
	//		status = false;
	//	}
	//}

	if (bConditionalActivation)
		status = bActivationConditionMet;
	else
		status = true;

	return status;
}

void AMovingPlatform::InitConditions()
{
	FConditionTableBase* currrow = new FConditionTableBase;

	for(TTuple<FName, unsigned char*> row : conditionTable->GetRowMap())
	{
		if(reinterpret_cast<FConditionTableBase*>(row.Value)->RowContent == rowContentID)
			currrow = reinterpret_cast<FConditionTableBase*>(row.Value);
	}

	for(TTuple<int, FConditionRowBase> rowconmap : currrow->ConditionMap)
	{
		if (rowconmap.Key == mapContentID)
		{
			activationCondition = rowconmap.Value;
			break;
		}
	}

	for (size_t i = 0; i < conditionButtons.Num(); i++)
	{
		// Eigenarbeit -> Erstellt einen Event Dispacher, die button sollen ihren owenr nicht kenner, der owenr soll aber wissen eine callback gebroadcastes haben bei activation
		conditionButtons[i]->SetButtonOwner(this, i);
	}
}

void AMovingPlatform::ResetConditions()
{
	for (AInteractableButton* button : conditionButtons)
	{
		button->SetMaterial_Invalid();

		if(currOwner)
			button->OnPlayerInteraction(currOwner->GetPlayerID());
	}

	activationCombination_Fill.Empty();
}

void AMovingPlatform::ConditionCallback(bool _status, int _objectID)
{
	Super::ConditionCallback(_status, _objectID);

	if(_status)
	{
		if (!activationCombination_Fill.Contains(_objectID))
			activationCombination_Fill.Add(_objectID);
	}
	else if(!_status)
	{

		if (activationCombination_Fill.Contains(_objectID))
		{
			activationCombination_Fill.Remove(_objectID);
			bActivationConditionMet = false;
		}
	}

	if(activationCombination_Fill.Num() == activationCondition.ConditionCombination.Num())
	{
		bool bvalid = true;

		for (size_t i = 0; i < activationCombination_Fill.Num(); i++)
		{
			if (activationCombination_Fill[i] != activationCondition.ConditionCombination[i])
				bvalid = false;
				break;
		}

		if (!bvalid)
			ResetConditions();

		bActivationConditionMet = bvalid;
	}
	else if(activationCombination_Fill.Num() > activationCondition.ConditionCombination.Num())
		ResetConditions();
	else if(activationCombination_Fill.Num() >= conditionButtons.Num())
		ResetConditions();
}
