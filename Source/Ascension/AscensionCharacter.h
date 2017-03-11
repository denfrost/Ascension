// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "Globals.h"
#include "AscensionCharacter.generated.h"


UCLASS(config=Game)
class AAscensionCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	AAscensionCharacter();

	/** Character's tick function. */
	virtual void Tick(float DeltaSeconds);

	/** State that the character is in. Determines whether the character can switch from one state to another. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character State")
	ECharacterState CharacterState;

	/** State of character's movement. Determines whether the character can walk/run/jump. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character State")
	EMovementState MovementState;

	/** State that the character's weapon is in. Determines whether the weapon is sheathed/unsheathed. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character State")
	EWeaponState WeaponState;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Speed at which the character walks. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed;

	/** Normal speed of the character. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalSpeed;

	/** Speed at which the character runs. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed;

	/** Force with which the character is launched forward. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float LaunchForwardForce;

	/** Force with which the character is launched upwards. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float LaunchUpwardForce;

	/** Meter tracking the player's attack combo count. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	int ComboMeter;

	/** Set to true if the character can chain an attack. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	bool CanChain;

protected:
	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Called for the player to sprint. */
	void Sprint();

	/** Called for the player to stop sprinting. */
	void StopSprinting();

	/** Called for the player to jump. */
	void Jump();

	/** Called for the player to stop jumping. */
	void StopJumping();

	/** Called for the player to perform an attack. */
	void Attack();

	/** Called for the player to dodge. */
	void Dodge();

	/** Called to stop player movement. */
	UFUNCTION(BlueprintCallable, Category = "Helper")
	void StopMovement();

	/** Called to reset player movement to normal speed. */
	UFUNCTION(BlueprintCallable, Category = "Helper")
	void ResetMovement();

	/** Called to check if the player can attack. */
	UFUNCTION(BlueprintCallable, Category = "Helper")
	bool CanAttack();

	/** Called to set parameters for launching the character. */
	UFUNCTION(BlueprintCallable, Category = "Helper")
	void SetLaunchParams(float LaunchHForce, float LaunchVForce);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	/** Attack anim montage. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	FPlayerAnimation LightAttack01;

	/** Attack anim montage. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	FPlayerAnimation LightAttack02;

	/** Attack anim montage. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	FPlayerAnimation LightAttack03;

	/** Attack anim montage. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	FPlayerAnimation StrongAttack01;

	/** Attack anim montage. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	FPlayerAnimation StrongAttack02;

	/** Attack anim montage. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	FPlayerAnimation StrongAttack03;

	/** Attack anim montage. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	FPlayerAnimation StrongAttack04;

	/** The dodge anim montage. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	FPlayerAnimation DodgeAnim01;

	/** The dodge anim montage. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* DodgeAnim;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

