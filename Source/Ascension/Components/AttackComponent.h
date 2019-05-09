// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Globals.h"
#include "Abilities/Attacks/Attack.h"
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
	void Initialize(const float ActionTurnRate);

protected:
	// Called when the component is in play.
	virtual void BeginPlay() override;

	// Called when the component exits play.
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
protected:
	/** Collision box that checks what actors are hit. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision")
	UBoxComponent* AttackHitBox;

	/** Array containing actors that have been damaged by this attack. 
	 * TODO: Move to Ability.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Damage")
	TArray<AActor*> DamagedActors;

public:
	/** Event that is broadcasted when an attack succeeds/fails. */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Event Dispatchers")
	FOnAttackComplete OnAttackComplete;

protected:
	/** Turn rate of the character when performing an action. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float ActionTurnRate;

	/** The direction the character should perform an action. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector ActionDirection;

public:
	/** Function to check whether this component's owner can attack. */
	UFUNCTION(BlueprintCallable, Category = "Validations")
	virtual bool CanAttack();

public:
	/** Called for the character to perform the specified attack. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attack")
	void Attack(const FString& AttackName, const FVector& MovementIntent);
	virtual void Attack_Implementation(const FString& AttackName, const FVector& MovementIntent);

	/** Function to select the attack to perform next. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	UAttack* SelectAttack(const FString& AttackType);
	virtual UAttack* SelectAttack_Implementation(const FString& AttackType);

	/** Event called when a combo is finished/reset.
	* Performs necessary actions after a combo is completed.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void Reset();
	virtual void Reset_Implementation();

	/** Scans and detects if the attack hits. */
	UFUNCTION(BlueprintCallable, Category = "Damage")
	void DetectHit();

	/** Clears the actors that have been damaged by the attack. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	void ClearDamagedActors();

public:
	/** Event called when character's movement needs to be set to flying.
	 * Sets movement mode to flying.
	 * TODO: Remove/Refactor to GameMovementComponent.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void SetFlyable();

	/** Event called when character's movement needs to be set to walking.
	 * Sets movement mode to walking.
	 * TODO: Remove/Refactor to GameMovementComponent.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void ResetFlyable();


	/** Finalizes character's attack direction. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void FinalizeAttackDirection(FVector MovementIntent);

protected:
	/** 
	 * Function to setup variables for attack motion.
	 * TODO: Refactor this to movement component entirely.
	 */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void SetupMotion();

	/** 
	 * Function that makes the character move in the specified direction.
	 * TODO: Refactor this to movement component entirely.
	 * @param MovementVector	Direction to move in.
	 */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void AttackMotion(FVector MovementVector);

	/**
	 * Function that resets variables to normal values when the attack completes.
	 * TODO: Refactor this to movement component entirely.
	 */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void FinishMotion();

protected:
	/**
	 * The currently active attack.
	 * TODO: Find a better way to do this.
	 */
	FString ActiveAttack;

	/** The component's owner. */
	UPROPERTY(VisibleAnywhere, Category = "Owner")
	ACharacter* Owner;
};
