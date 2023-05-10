// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WinConditionManager.generated.h"

UCLASS()
class AWinConditionManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWinConditionManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = WinCondition, meta = (AllowPrivateAccess))
		class AInteractableButton* winButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = WinCondition, meta = (AllowPrivateAccess))
		TSubclassOf<AInteractableButton> winButtonSubClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = WinCondition, meta = (AllowPrivateAccess))
		UUserWidget* winScreenWidgetSubclass;

	UPROPERTY()
	bool bGameWon;

	UFUNCTION()
	void SetGameWon(int _playerID);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE
	bool GetHasWon() { return bGameWon; }

	UFUNCTION(CallInEditor, Category=WinCondition)
	void SpawnWinButton();
};
