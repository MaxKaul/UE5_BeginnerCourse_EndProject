#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ConditionLibrary.generated.h"

// Darauf eingehen das wir das so machen damit man später noch andere arten kombinations arten im strcut haben kann
USTRUCT(BlueprintType)
struct FConditionRowBase : public FTableRowBase
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere)
		TArray<int> ConditionCombination;

	FConditionRowBase()
	{

	};
};

// Auf the .generated eingehen
USTRUCT(BlueprintType)
struct FConditionTableBase : public FTableRowBase
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere)
		FName RowContent;

		UPROPERTY(EditAnywhere)
		TMap<int , FConditionRowBase> ConditionMap;

	FConditionTableBase()
	{

	};
};