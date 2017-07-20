// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Classes/Components/TimelineComponent.h"
#include "Globals.h"
#include "AttackComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttackComplete, bool, Successful);


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASCENSION_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttackComponent();
	
	/** Function called to initialize the attack component.
	  * To be called from main entity to initialize these variables.
	  */
	UFUNCTION(BlueprintCallable, Category = "Initialization")
	void Initialize(const float NormalSpeed, const float NormalAcceleration, const float NormalTurnRate, const float ActionTurnRate);

protected:
	// Called when the component is in play.
	virtual void BeginPlay() override;

	// Called when the component exits play.
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
protected:
	/** Array containing the attacks.
	* Do NOT directly modify this. Use CreateAttack to add attacks to this instead.
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attacks")
	TArray<FAttack> Attacks;

	/** Array containing movement timlines.
	* Do NOT directly modify this. Use CreateAttack to add attack timelines to this instead.
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Timelines")
	TArray<UTimelineComponent*> AttackTimelines;

	/** Structure that maps attack names to their respective indices.
	* Do NOT directly modify this. Use CreateAttack to add attack name - index values to this instead.
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attacks")
	TMap<FString, int> AttackNameMap;

	/** Collision box that checks what actors are hit. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision")
	UBoxComponent* AttackHitBox;

	/** Array containing actors that have been damaged by this attack. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Damage")
	TArray<AActor*> DamagedActors;

public:
	/** Event that is broadcasted when an attack succeeds/fails. */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Event Dispatchers")
	FOnAttackComplete OnAttackComplete;

protected:
	/** Attack to be performed. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animations")
	FAttack AttackToPerform;

	/** Used for comparison purposes. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animations")
	FAttack NullAttack;

protected:
	/** The timeline to be played. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	UTimelineComponent* TimelineToPlay;

	/** Timer handle used for checking if characters are hit during the attack. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Timers")
	FTimerHandle DamageTickTimerHandle;

	/** Delay between damage ticks. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timers")
	float DamageTickDelay = 0.025f;

protected:
	/** Normal speed of the character. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalSpeed;

	/** Acceleration of the character. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalAcceleration;

	/** Base turn rate of the charater. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalTurnRate;

	/** Turn rate of the character when performing an action. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float ActionTurnRate;

	/** The direction the character should perform an action. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector ActionDirection;

public:
	/** Creates an attack and adds it's details to the respective lists. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Initialization")
	void CreateAttack(const FString& AttackName, const FAttack& Attack);

	/** Retrieves the specified attack. Found is set to true if the attack is valid. */
	UFUNCTION(BlueprintCallable, Category = "Helper")
	void GetAttack(const FString& AttackName, bool& Found, FAttack& Attack, UTimelineComponent*& AttackTimeline);

	/** Sets the specified attack as the attack to perform and sets movement-related parameters. */
	UFUNCTION(BlueprintCallable, Category = "Helper")
	void SetAttack(const bool& Found, const FAttack& Attack, UTimelineComponent* AttackTimeline);

public:
	/** Called for the character to perform the specified attack. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attack")
	void Attack(const FString& AttackName, const FVector& MovementIntent);
	virtual void Attack_Implementation(const FString& AttackName, const FVector& MovementIntent);

	/** Function to select the attack to perform next. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void SelectAttack(const FString& AttackType);
	virtual void SelectAttack_Implementation(const FString& AttackType);

	/** Event called when a combo is finished/reset.
	* Performs necessary actions after a combo is completed.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void Reset();
	virtual void Reset_Implementation();

	/** Plays the timeline for attack movement. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void AttackMovement();

	/** Stops the timeline for attack movement. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void StopAttackMovement();

	/** Scans and detects if the attack hits. */
	UFUNCTION(BlueprintCallable, Category = "Damage")
	void DetectHit();

	/** Clears the actors that have been damaged by the attack. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	void ClearDamagedActors();

public:
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


	/** Finalizes character's attack direction. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void FinalizeAttackDirection(FVector MovementIntent);

protected:
	/** Called to stop character movement. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void StopMovement();

	/** Called to limit character movement to a certain speed. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetMovementSpeed(float Speed);

	/** Called to reset character movement to normal speed. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ResetMovementSpeed();

	/** Called to set acceleration to a value. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetAcceleration(float Acceleration);

	/** Called to reset acceleration. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ResetAcceleration();

	/** Called to stop character turning. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void StopTurning();

	/** Called to limit character turning to a certain rate. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetTurningRate(float Rate);

	/** Called to reset character turning to normal rate. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ResetTurningRate();

	/** Called to set gravity to a value. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetGravity(float GravityValue);

	/** Called to reset gravity. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ResetGravity();

protected:
	/** Function to handle the movement of the character in the timeline (front/back movement). */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void TimelineMovement(float Speed);

	/** Function to handle the 2D movement of the character in the timeline (forward/side movement). */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void TimelineMovement2D(FVector MovementVector);

	/** Function to handle the 3D movement of the character in the timeline (front/side/up movement). */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void TimelineMovement3D(FVector MovementVector);

	/** A function that sets up the timeline for an attack's movement (1D). */
	UFUNCTION(BlueprintCallable, Category = "Helper")
	void SetupTimelineComponent(UTimelineComponent* TimelineComponent, UCurveFloat* MovementCurve, float Duration);

	/** A function that sets up the timeline for an attack's movement (2D). */
	UFUNCTION(BlueprintCallable, Category = "Helper")
	void SetupTimelineComponent2D(UTimelineComponent* TimelineComponent, UCurveVector* MovementCurve, float Duration);

	/** A function that sets up the timeline for an attack's movement (3D). */
	UFUNCTION(BlueprintCallable, Category = "Helper")
	void SetupTimelineComponent3D(UTimelineComponent* TimelineComponent, UCurveVector* MovementCurve, float Duration);

private:
	/** The component's owner. */
	UPROPERTY(VisibleAnywhere, Category = "Owner")
	ACharacter* Owner;
};
