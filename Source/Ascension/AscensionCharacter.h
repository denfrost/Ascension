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

	/** Used to indicate to the animation blueprint whether the character should switch weapons. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool ShouldCharSwitch;

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

	/** Called for the player to perform a light attack. */
	void LightAttack();

	/** Called for the player to perform a strong attack. */
	void StrongAttack();

	/** Called for the player to dodge. */
	void Dodge();

	/** Called for the character to sheath/unsheath weapon. */
	void SwitchWeapon();

	/** Called to stop player movement. */
	UFUNCTION(BlueprintCallable, Category = "Helper")
	void StopMovement();

	/** Called to limit player movement to a certain speed. */
	UFUNCTION(BlueprintCallable, Category = "Helper")
	void LimitMovement(float Speed);

	/** Called to reset player movement to normal speed. */
	UFUNCTION(BlueprintCallable, Category = "Helper")
	void ResetMovement();

	/** Called to stop player turning. */
	UFUNCTION(BlueprintCallable, Category = "Helper")
	void StopTurning();

	/** Called to limit player turning to a certain rate. */
	UFUNCTION(BlueprintCallable, Category = "Helper")
	void LimitTurning(float Rate);

	/** Called to reset player turning to normal rate. */
	UFUNCTION(BlueprintCallable, Category = "Helper")
	void ResetTurning();

	/** Called to set gravity to a value. */
	UFUNCTION(BlueprintCallable, Category = "Helper")
	void SetGravity(float GravityValue);

	/** Called to reset gravity. */
	UFUNCTION(BlueprintCallable, Category = "Helper")
	void ResetGravity();

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
	// TODO: Move attacks into a separate component.

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

	FPlayerAnimation SelectAttack(FString AttackType);

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:
	/** Event called when a combo is finished/reset.
	  * Performs necessary actions after a combo is completed.
	  */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void ResetCombo();

	/** Event called when a dodge is finished.
	 * Performs necessary actions after a dodge is completed.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void ResetDodge();

	/** Event called when the character completes switching.
	  * Performs necessary actions after a switch is completed.
	  */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void SwitchComplete();

	/** Event called when rotation rate needs to be limited.
	  * Limits rotation rate.
	  */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void LimitTurn();

	/** Event called when rotation rate needs to be reset.
	  * Resets rotation rate.
	  */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void ResetTurn();

	/** Event called when character needs to be launch.
	  * Launches the character.
	  */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void LaunchChar();

	/** Event called when character can chain an attack.
	  * Sets CanChainAttack.
	  */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void CanChainAttack();

	/** Event called when character's movement needs to be set to flying.
	  * Sets movement mode to flying.
	  */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void SetFlyable();

	/** Event called when character's movement needs to be set to walking.
	* Sets movement mode to walking.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void ResetFlyable();

public:
	// The following are events whose functionality is implemented in blueprint.

	/** Event called when the sword is sheathed. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void Sheathed();

	/** Event called when the sword is unsheathed. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void Unsheathed();

	/** Event called to play footstep sounds. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void FootstepSound();

	/** Event called to play a sword slash sound. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void SlashSound();
};

