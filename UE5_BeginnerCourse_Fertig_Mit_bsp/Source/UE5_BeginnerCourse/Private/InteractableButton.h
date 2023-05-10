// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableBase.h"
#include "InteractableButton.generated.h"

UCLASS()
class AInteractableButton : public AInteractableBase
{
	GENERATED_BODY()

private:

	UPROPERTY()
		int buttonID;

	TOptional<int> currControllerID;

	UPROPERTY()
		AInteractableBase* buttonOwner;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	UMaterialInterface* validMat;
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	UMaterialInterface* invalidMat;

public:
	FORCEINLINE
		void SetButtonOwner(AInteractableBase* _owner, int _id) { buttonOwner = _owner; buttonID = _id; }

	virtual void OnPlayerInteraction(int _interactorID) override;

	UFUNCTION()
	virtual void SetMaterial_Invalid();
	UFUNCTION()
	virtual void SetMaterial_Valid();

	FORCEINLINE
		TOptional<int>  GetCurrControllerID() { return currControllerID; }
};
