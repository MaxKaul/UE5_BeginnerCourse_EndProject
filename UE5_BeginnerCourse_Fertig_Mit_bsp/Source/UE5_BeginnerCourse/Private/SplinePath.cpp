// Fill out your copyright notice in the Description page of Project Settings.


#include "SplinePath.h"

#include "Components/SplineComponent.h"

ASplinePath::ASplinePath()
{
	PrimaryActorTick.bCanEverTick = true;

	pathSpline = CreateDefaultSubobject<USplineComponent>("PathSpline");
	RootComponent = pathSpline;
}

void ASplinePath::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASplinePath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

