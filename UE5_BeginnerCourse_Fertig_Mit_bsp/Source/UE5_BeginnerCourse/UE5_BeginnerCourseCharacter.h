// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "UE5_BeginnerCourseCharacter.generated.h"


UCLASS(config=Game)
class AUE5_BeginnerCourseCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* LookActionEnd;

	UPROPERTY(VisibleAnywhere, Category = Input)
	float currYawInput;

	UPROPERTY(EditAnywhere, Category = Stats, meta=(AllowPrivateAccess))
	float health;

	UPROPERTY(VisibleAnywhere,Category = Components)
		class UPlayerInteractor* playerInteractor;

	UPROPERTY(VisibleAnywhere, Category = Components)
		class AInteractableBase* currInteractObject;

	UFUNCTION()
		void PlayDeath();

	UFUNCTION()
		void CleanAfterDeath();

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category=Components,meta=(AllowPrivateAccess))
		TSubclassOf<class ABCPlayerStart> playerSpawnSubclass;
	UPROPERTY()
		ABCPlayerStart* playerSpawn;

	UPROPERTY()
		bool bHasDied;

	UPROPERTY()
		int playerID;

public:
	AUE5_BeginnerCourseCharacter();

	FORCEINLINE
		void SetCurrInteractable(AInteractableBase* _currInter) { currInteractObject = _currInter; }

	UFUNCTION()
		void TakeDamage(float _damage);

	UFUNCTION(BlueprintPure) FORCEINLINE
		bool PlayerDied() { return bHasDied; }

	UFUNCTION(BlueprintPure) FORCEINLINE
		float GetCurrentHealth() { return health; }

	FORCEINLINE
		int GetPlayerID() { return playerID; };

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	void LookEnd(const FInputActionValue& Value);

	UFUNCTION()
	void Interact();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


	FORCEINLINE float GetCurrYawInput() { return currYawInput; }
};