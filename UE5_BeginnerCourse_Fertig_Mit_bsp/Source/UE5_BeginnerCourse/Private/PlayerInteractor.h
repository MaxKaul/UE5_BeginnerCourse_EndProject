// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInteractor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UPlayerInteractor : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerInteractor();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		void TraceInteractables();

	UPROPERTY()
		UWorld* world;

	UPROPERTY()
		class AUE5_BeginnerCourseCharacter* componentOnwer;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Components, meta=(AllowPrivateAccess))
		FVector2D crosshairSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats, meta = (AllowPrivateAccess))
		float interactableRange;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Components, meta=(AllowPrivateAccess))
		UStaticMeshComponent* viewPoint;

	UPROPERTY()
		TArray<UMaterialInterface*> allMaterials;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess))
		UMaterialInterface* translucentMat;
};