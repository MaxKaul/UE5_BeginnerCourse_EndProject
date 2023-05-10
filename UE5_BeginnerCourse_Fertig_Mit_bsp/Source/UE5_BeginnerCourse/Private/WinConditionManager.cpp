// Fill out your copyright notice in the Description page of Project Settings.


#include "WinConditionManager.h"

#include "BCGameInstance.h"
#include "InteractableButton.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWinConditionManager::AWinConditionManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWinConditionManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWinConditionManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (winButton && winButton->WasInteracted() && !bGameWon)
	{
		if(winButton->GetCurrControllerID().IsSet())
			SetGameWon(winButton->GetCurrControllerID().GetValue());
		else
			UE_LOG(LogTemp, Warning, TEXT("AWinConditionManager, !GetCurrControllerID.IsSet"))
	}
}

void AWinConditionManager::SetGameWon(int _playerID)
{
	bGameWon = true;
	UGameplayStatics::GetPlayerControllerFromID(GetWorld(), 0)->UnPossess();
	UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());

	if(!winScreenWidgetSubclass)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWinConditionManager, !winScreenWidgetSubclass"))
		return;
	}

	winScreenWidgetSubclass->AddToViewport();

	FInputModeUIOnly inputmode;
	UGameplayStatics::GetPlayerControllerFromID(GetWorld(), _playerID)->SetInputMode(inputmode);
	UGameplayStatics::GetPlayerControllerFromID(GetWorld(), _playerID)->SetShowMouseCursor(true);
}

void AWinConditionManager::SpawnWinButton()
{
	if(!winButtonSubClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWinConditionManager, !winButtonSubClass"))
		return;
	}

	if (!winButton)
		winButton = GetWorld()->SpawnActor<AInteractableButton>(winButtonSubClass, GetActorLocation(), GetActorRotation());
	else
		UE_LOG(LogTemp,Warning,TEXT("Button Already Spawned!"))
}