// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInteractor.h"

#include "InteractableButton.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UE5_BeginnerCourse/UE5_BeginnerCourseCharacter.h"

UPlayerInteractor::UPlayerInteractor()
{
	PrimaryComponentTick.bCanEverTick = true;

	crosshairSize = FVector2D(5.f, 5.f);
	interactableRange = 1000.f;

	viewPoint = CreateDefaultSubobject<UStaticMeshComponent>("viewPoint");
}


// Called when the game starts
void UPlayerInteractor::BeginPlay()
{
	Super::BeginPlay();

	world = GetWorld();

	componentOnwer = Cast<AUE5_BeginnerCourseCharacter>(GetOwner());

	// Warum AttackToComp u. nicht setupattachement
	viewPoint->AttachToComponent(Cast<AUE5_BeginnerCourseCharacter>(componentOnwer)->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Interactable_Socket"));

	for (UMaterialInterface* mat : viewPoint->GetMaterials())
	{
		allMaterials.Add(mat);
	}
}


// Called every frame
void UPlayerInteractor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!world || !componentOnwer)
		return;

	TraceInteractables();
}

void UPlayerInteractor::TraceInteractables()
{
	FVector worlddir;
	FVector worldpos;

	// Leiten sich aus der screen position ab
	float crosshairPosX = UWidgetLayoutLibrary::GetViewportSize(world).X / 2;
	float crosshairPosY = UWidgetLayoutLibrary::GetViewportSize(world).Y / 3;

	crosshairPosX -= crosshairSize.X;
	crosshairPosY -= crosshairSize.Y;

	FVector2D viewPortPos = FVector2D(crosshairPosX, crosshairPosY);

	UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(world, 0), viewPortPos, worldpos, worlddir);

	FHitResult hit;

	const TArray<AActor*> emptyActor = { componentOnwer };

	UKismetSystemLibrary::LineTraceSingleByProfile(world, worldpos, worldpos + worlddir * interactableRange, "BlockAll",
		false, emptyActor, EDrawDebugTrace::None, hit, true, FColor::Transparent, FColor::Transparent, 0.f);

	if(hit.bBlockingHit && !componentOnwer->PlayerDied())
	{
		viewPoint->SetWorldLocation(hit.ImpactPoint, false, nullptr, ETeleportType::None);

		if (AInteractableButton* button = Cast<AInteractableButton>(hit.GetActor()))
			componentOnwer->SetCurrInteractable(button);
		else
			componentOnwer->SetCurrInteractable(nullptr);

		for (size_t i = 0; i < allMaterials.Num(); i++)
		{
			if(allMaterials[i])
				viewPoint->SetMaterial(i, allMaterials[i]);
		}
	}
	else
	{
		if(!translucentMat)
		{
			UE_LOG(LogTemp,Warning,TEXT("UPlayerInteractor, !translucentMat"))
			return;
		}

		for (size_t i = 0; i < viewPoint->GetMaterials().Num(); i++)
		{
			viewPoint->SetMaterial(i, translucentMat);
		}
	}
}
