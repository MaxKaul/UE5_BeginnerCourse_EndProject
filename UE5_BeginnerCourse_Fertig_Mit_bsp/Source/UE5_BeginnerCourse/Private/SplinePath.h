// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SplinePath.generated.h"

UCLASS()
class ASplinePath : public AActor
{
	GENERATED_BODY()
	
public:	
	ASplinePath();

	FORCEINLINE
		class USplineComponent* GetSplinePath() { return pathSpline; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess))
		 USplineComponent* pathSpline;
};
