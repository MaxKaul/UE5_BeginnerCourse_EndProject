// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableButton.h"

void AInteractableButton::OnPlayerInteraction(int _interactorID)
{
	Super::OnPlayerInteraction(_interactorID);

	if(buttonOwner)
		buttonOwner->ConditionCallback(bIsInteracted, buttonID);

	if (bIsInteracted)
	{
		currControllerID = _interactorID;
		SetMaterial_Valid();
	}
	else if(!bIsInteracted)
	{
		currControllerID.Reset();
		SetMaterial_Invalid();
	}
}

void AInteractableButton::SetMaterial_Valid()
{
	for (size_t i = 0; i < objectMesh->GetMaterials().Num(); i++)
	{
		objectMesh[i].SetMaterial(i, validMat);
	}
}

void AInteractableButton::SetMaterial_Invalid()
{
	for (size_t i = 0; i < objectMesh->GetMaterials().Num(); i++)
	{
		objectMesh[i].SetMaterial(i, invalidMat);
	}
}

