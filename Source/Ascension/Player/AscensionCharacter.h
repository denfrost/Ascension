// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "Globals.h"
#include "Classes/Components/TimelineComponent.h"
#include "Interfaces/Damageable.h"
#include "AscensionCharacter.generated.h"


/** AscensionCharacter
  *	This is the character that the player controls and plays the game with.
  */
UCLASS(config=Game)
class AAscensionCharacter : public ACharacter, public IDamageable
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Component handling attacks. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UPlayerAttackComponent* AttackComponent;

public:
	AAscensionCharacter();

	/** Character's BeginPlay function. */
	virtual void BeginPlay();

	/** Character's Tick function. */
	virtual void Tick(float DeltaSeconds);

protected:
	/** State that the character is in. Determines whether the character can switch from one state to another. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character State")
	ECharacterState CharacterState;

	/** State of character's movement. Determines whether the character can walk/run/jump. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character State")
	EMovementState MovementState;

	/** State that the character's weapon is in. Determines whether the weapon is sheathed/unsheathed. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character State")
	EWeaponState WeaponState;

public:
	/** Gets the character state. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "State Helper")
	ECharacterState GetCharacterState() const;

	/** Gets the movement state. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "State Helper")
	EMovementState GetMovementState() const;

	/** Gets the weapon state. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "State Helper")
	EWeaponState GetWeaponState() const;

public:
	/** Current health of the character.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Parameters")
	float Health;

	/** Maximum health of the character.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Parameters")
	float MaxHealth;

	/** Set to true when the character is dead. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character Parameters")
	bool Dead;


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

	/** Acceleration of the character. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalAcceleration;

	/** Base turn rate of the charater. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalTurnRate;

	/** Turn rate of the character when performing an action. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float ActionTurnRate;

	/** Set to true if the character can chain an attack. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	bool CanChain;

	/** Used to indicate to the animation blueprint whether the character should switch weapons. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool ShouldCharSwitch;

	/** Used to check if the character can be moved with player input. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool CanMove;

	/** The vector which holds player's forward movement intent. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector ForwardIntent;

	/** The vector which holds player's side movement intent. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector SideIntent;

	/** The vector holding the player's movement intent. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector MovementIntent;

	/** The direction the player should perform an action. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector ActionDirection;

	/** The timeline to be played. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	UTimelineComponent* TimelineToPlay;


	/** Variable to detect if player is locked on to an enemy. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	bool LockedOn;

	/** The enemy that the player is locked on to. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	AActor* LockedActor;

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
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void Sprint();

	/** Called for the player to stop sprinting. */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StopSprinting();

	/** Called for the player to jump. */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void Jump();

	/** Called for the player to stop jumping. */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StopJumping();

	/** Called for the player to perform a light attack. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void LightAttack();

	/** Called for the player to perform a strong attack. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void StrongAttack();

	/** Called for the player to dodge. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void Dodge();

	/** Called for the character to sheath/unsheath weapon. */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void SwitchWeapon();

	/** Called to enable movement through player input. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void EnableMovement();

	/** Called to disable movement through player input. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void DisableMovement();

	/** Called to stop player movement. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void StopMovement();

	/** Called to limit player movement to a certain speed. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetMovementSpeed(float Speed);

	/** Called to reset player movement to normal speed. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ResetMovementSpeed();

	/** Called to set acceleration to a value. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetAcceleration(float Acceleration);

	/** Called to reset acceleration. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ResetAcceleration();

	/** Called to stop player turning. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void StopTurning();

	/** Called to limit player turning to a certain rate. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetTurningRate(float Rate);

	/** Called to reset player turning to normal rate. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ResetTurningRate();

	/** Called to set gravity to a value. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetGravity(float GravityValue);

	/** Called to reset gravity. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ResetGravity();

	/** Called to check if the player can attack. */
	UFUNCTION(BlueprintCallable, Category = "Helper")
	bool CanAttack();

	/** Function to handle the movement of the character in the timeline. */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void TimelineMovement(float Speed);

	/** A function that sets up the timeline for an attack/dodge move. */
	UFUNCTION(BlueprintCallable, Category = "Helper")
	void SetupTimelineComponent(UTimelineComponent* TimelineComponent, UCurveFloat* MovementCurve);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	/** Dodge anim montage. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	FPlayerAnimation DodgeMove;

	/** A timeline for movement. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timelines")
	UTimelineComponent* DodgeTimeline;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	/** Function that returns the percentage of health the character has left. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Getters")
	float GetHealthPercentage() const;

	/** Function that checks and sets whether the character is dead. */
	UFUNCTION(BlueprintCallable, Category = "Checking")
	bool CheckIsDead();

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
	void ResetAttack();

	/** Event called when a dodge is finished.
	 * Performs necessary actions after a dodge is completed.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void ResetDodge();

	/** Plays the timeline for dodge movement. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void DodgeMovement();

	/** Plays the timeline for attack movement. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void AttackMovement();

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

	/** Function that resets hit detection. */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void ResetDetection();

	/** Function to perform hit detection. */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void DetectHit();

	/** Finalizes player's attack direction. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void FinalizeAttackDirection();

	/** Event called when character is hit by something. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void Impact(const FVector& Direction);

	/** Event called when recovering from an impact. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void Recovered();

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

public:
	/* DAMAGEABLE INTERFACE FUNCTIONS */

	/** Returns the percentage of health remaining. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface Functions")
	void GetHealthPercent(float& HealthPercent);
	virtual void GetHealthPercent_Implementation(float& HealthPercent) override;

	/** Applies the effects of an attack to the entity. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface Functions")
	void ApplyHitEffect(const AActor* SourceActor, const float Damage, const EHitEffect HitEffect, const FAttackEffect AttackEffect);
	virtual void ApplyHitEffect_Implementation(const AActor* SourceActor, const float Damage, const EHitEffect HitEffect, const FAttackEffect AttackEffect) override;

	/** Shows the entity's health bar. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface Functions")
	void ShowHealthBar();
	virtual void ShowHealthBar_Implementation() override;

	/** Hides the entity's health bar. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface Functions")
	void HideHealthBar();
	virtual void HideHealthBar_Implementation() override;

	/** To check whether the entity is alive or not. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface Functions")
	bool IsDead();
	virtual bool IsDead_Implementation() override;
};

