// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Ability.h"
#include "Abilities/Attacks/AttackGlobals.h"
#include "Interfaces/GameMovementInterface.h"
#include "Attack.generated.h"

/*
 * Class for implementing attacks which can be performed by entities.
 */
UCLASS(Blueprintable, ClassGroup = (Attacks))
class ASCENSION_API UAttack : public UAbility
{
	GENERATED_BODY()
	
public:
	/*
	 * Constructor of the attack.
	 */
	UAttack();

	/*
	 * This method activates the ability.
	 */
	virtual void Activate();

protected:
	// TODO: Move anim montages, attack movement/effects out to separate subclasses if necessary.

	/** Animation montage of the attack. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Variables")
	UAnimMontage* AnimMontage;

	/*
	 * Movement parameters for the attack. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Variables")
	FCustomMovementParams MovementParams;

	/*
	 * Details of the attacks effects.
	 * TODO: Rework this.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
	FAttackEffectInfo EffectInfo;

public:
	/** Function to get the movement direction. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface Functions")
	FCustomMovementParams GetMovementParams() const;
	virtual FCustomMovementParams GetMovementParams_Implementation() const;

	/*
	 * Gets the effect info of the attack.
	 */
	UFUNCTION(BlueprintCallable, Category = "Getters")
	FAttackEffectInfo GetEffectInfo() const;
};
