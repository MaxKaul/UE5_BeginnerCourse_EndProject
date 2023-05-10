// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "BCPlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class ABCPlayerStart : public APlayerStart
{
	GENERATED_BODY()


private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess))
		TSubclassOf<class AUE5_BeginnerCourseCharacter> player;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess))
		float respawnTime;

public:
	void RespawnPlayer(class AUE5_BeginnerCourseCharacter* _oldPlayer);

	FORCEINLINE
		float GetRespawnTime() { return respawnTime; }
};
