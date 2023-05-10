// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableBase.generated.h"


DECLARE_DYNAMIC_DELEGATE_RetVal_SixParams(bool, FBoolReturnFunc, UPrimitiveComponent*, _overlappedComponent, AActor*, _otherActor, UPrimitiveComponent*, _otherComp, int32, _otherBodyIndex, bool, _bFromSweep, const FHitResult&, _sweepResult);

UCLASS()
class AInteractableBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AInteractableBase();


	UFUNCTION()
		virtual void OnPlayerInteraction(int _playerID);

	FORCEINLINE
		bool WasInteracted() { return bIsInteracted; }

	UFUNCTION()
		virtual void ConditionCallback(bool _status, int _objectID);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		virtual bool PlayerInteraction_OnOverlapBegin(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult);
	UFUNCTION()
		virtual bool PlayerInteraction_OnOverlapEnd(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex);


	UFUNCTION()
		virtual bool ConditionCheck();
	UFUNCTION()
		virtual void InitConditions();
	UFUNCTION()
		virtual void ResetConditions();

	UPROPERTY()
	FBoolReturnFunc overlapBegin;
	UPROPERTY()
	FBoolReturnFunc overlapEnd;

	UPROPERTY(VisibleAnywhere, Category = Status)
		bool bIsInteracted;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess))
		UStaticMeshComponent* objectMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess))
		class UBoxComponent* objectHitBox;
};