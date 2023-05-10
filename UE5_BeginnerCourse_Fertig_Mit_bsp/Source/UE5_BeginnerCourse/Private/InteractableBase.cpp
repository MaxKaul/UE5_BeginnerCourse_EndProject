// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableBase.h"

#include "Components/BoxComponent.h"
#include "UE5_BeginnerCourse/UE5_BeginnerCourseCharacter.h"

AInteractableBase::AInteractableBase()
{
	PrimaryActorTick.bCanEverTick = true;

	objectMesh = CreateDefaultSubobject<UStaticMeshComponent>("ObjectMesh");
	RootComponent = objectMesh;

	objectHitBox = CreateDefaultSubobject<UBoxComponent>("ObjectHitBox");
	objectHitBox->SetupAttachment(RootComponent);
}

void AInteractableBase::BeginPlay()
{
	Super::BeginPlay();

	overlapBegin.BindUFunction(this, "PlayerInteraction_OnOverlapBegin");
	overlapEnd.BindUFunction(this, "PlayerInteraction_OnOverlapEnd");
	
	objectHitBox->OnComponentBeginOverlap.Add(overlapBegin);
	objectHitBox->OnComponentEndOverlap.Add(overlapEnd);
}

void AInteractableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


bool AInteractableBase::PlayerInteraction_OnOverlapBegin(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult)
{
	bool status = false;

	// War vorher Cast<CharatcerClass>
	if (!_otherComp->ComponentHasTag("Interactor"))
	{
		if (AUE5_BeginnerCourseCharacter* player = Cast<AUE5_BeginnerCourseCharacter>(_otherActor))
		{
			OnPlayerInteraction(player->GetPlayerID());
			status = true;
		}
	}

	return status;
}

bool AInteractableBase::PlayerInteraction_OnOverlapEnd(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex)
{
	bool status = false;

	// War vorher Cast<CharatcerClass>
	if (!_otherComp->ComponentHasTag("Interactor"))
	{
		if (AUE5_BeginnerCourseCharacter* player = Cast<AUE5_BeginnerCourseCharacter>(_otherActor))
		{
			OnPlayerInteraction(player->GetPlayerID());
			status = true;
		}
	}

	return status;
}

void AInteractableBase::OnPlayerInteraction(int _playerID)
{
	bIsInteracted = !bIsInteracted;
}

bool AInteractableBase::ConditionCheck() { return false; }

void AInteractableBase::ConditionCallback(bool _status ,int _objectID) { }

void AInteractableBase::InitConditions() {}

void AInteractableBase::ResetConditions() {}