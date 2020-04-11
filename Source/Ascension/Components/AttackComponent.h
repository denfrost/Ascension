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
	void Initialize();

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
	/** The direction the character should perform an action. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector ActionDirection;

public:
	/** Called for the character to perform the specified attack. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attack")
	void Attack(const FString& AttackName, const FVector& MovementIntent);
	virtual void Attack_Implementation(const FString& AttackName, const FVector& MovementIntent);

	/*
	 * TODO: Refactor this method. We should have the option to either specify attack ID or specify the attack name.
	 * Called for the character to finish the specified attack.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attack")
	void FinishAttack(const FString& AttackName);
	virtual void FinishAttack_Implementation(const FString& AttackName);

	/** Scans and detects if the attack hits. */
	UFUNCTION(BlueprintCallable, Category = "Damage")
	void DetectHit();

	/** Clears the actors that have been damaged by the attack. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	void ClearDamagedActors();

public:
	/** Finalizes character's attack direction. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void FinalizeAttackDirection(FVector MovementIntent);

protected:
	/*
	 * Function to setup variables for attack motion.
	 * TODO: Refactor this to movement component entirely.
	 */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void SetupMotion();

	/*
	 * Function that makes the character move in the specified direction.
	 * TODO: Refactor this to movement component entirely.
	 * @param MovementVector	Direction to move in.
	 */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void AttackMotion(FVector MovementVector);

	/*
	 * Function that resets variables to normal values when the attack completes.
	 * TODO: Refactor this to movement component entirely.
	 */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void FinishMotion();

protected:
	/*
	 * Map of active attack names to their IDs.
	 * TODO: Refactor so that multiple attack IDs can exist for a particular attack name.
	 */
	TMap<FString, uint32> ActiveAttackIDMap;

	/** The component's owner. */
	UPROPERTY(VisibleAnywhere, Category = "Owner")
	ACharacter* Owner;
};
