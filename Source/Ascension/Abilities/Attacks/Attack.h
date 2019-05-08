// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Ability.h"
#include "AttackGlobals.h"
#include "Attack.generated.h"

/**
 * Class for implementing attacks which can be performed by entities.
 */
UCLASS(Blueprintable, ClassGroup = (Attacks))
class ASCENSION_API UAttack : public UAbility
{
	GENERATED_BODY()
	
public:
	/**
	 * Constructor of the attack.
	 */
	UAttack(const FObjectInitializer& ObjectInitializer);

	/**
	 * This method activates the ability.
	 */
	virtual void Activate();

public:
	/**
	 * Function to check whether an attack can be activated.
	 */
	virtual bool CanActivate() const = 0;

protected:
	/** Animation montage of the attack. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Variables")
	UAnimMontage* AnimMontage;

	/** Details of movement during attacks. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Variables")
	FAttackMovementInfo MovementInfo;

	/**
	 * Details of the attacks effects.
	 * TODO: Rework this.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Variables")
	FAttackEffectInfo EffectInfo;

public:
	/**
	 * Gets the movement info of the attack.
	 * TODO: Remove this.
	 */
	UFUNCTION(BlueprintCallable, Category = "Getters")
	FAttackMovementInfo GetMovementInfo() const;

	/**
	 * Gets the effect info of the attack.
	 * TODO: Remove this.
	 */
	UFUNCTION(BlueprintCallable, Category = "Getters")
	FAttackEffectInfo GetEffectInfo() const;
};
