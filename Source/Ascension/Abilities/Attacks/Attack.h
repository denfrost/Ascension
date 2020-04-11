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
	UAttack(FString Name, class UGameAbilitySystemComponent* System);

	/**
	  * This method activates the ability.
	  */
	virtual void Activate(FVector AttackDirection);

protected:
	// TODO: Move anim montages, attack movement/effects out to separate subclasses if necessary.

	/** Animation montage of the attack. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Variables")
	UAnimMontage* AnimMontage;

	/** Details of movement during attacks. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	FAttackMovementInfo MovementInfo;

	/** The direction the attack should be performed. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector AttackDirection;

	/**
	  * Details of the attacks effects.
	  * TODO: Rework this.
	  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
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
