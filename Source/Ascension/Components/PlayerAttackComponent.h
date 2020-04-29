// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/AttackComponent.h"
#include "Abilities/Attacks/Attack.h"
#include "PlayerAttackComponent.generated.h"


UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ASCENSION_API UPlayerAttackComponent : public UAttackComponent
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties.
	UPlayerAttackComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	/** Tracks how many attacks the user has chained. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combos")
	int ComboMeter;

	/** Maximum number of attacks in a combo. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combos")
	int MaxComboCount;

public:
	/*
	 * Implementation for selecting attacks.
	 * @param AttackType	Type of attack to perform.
	 * @returns FString		Name of attack ability to perform.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	FString SelectAttack(const FString& AttackType);
	virtual FString SelectAttack_Implementation(const FString& AttackType);

	/*
	 * Called for the character to setup the specified attack.
	 * @param AttackName	Name of the attack to setup.
	 */
	virtual void SetupAttack_Implementation(const FString& AttackName);

	/*
	 * Called for the character to perform the specified attack.
	 * @param AttackName	Name of the attack to perform.
	 * @returns bool		Whether the attack was executed.
	 */
	virtual bool Attack_Implementation(const FString& AttackName);

	/*
	 * Called for the character to finish the specified attack.
	 * @param AttackName	Name of the attack to finish.
	 */
	virtual void FinishAttack_Implementation(const FString& AttackName);

	/*
	 * Event called when a combo is finished/reset.
	 * Performs necessary actions after a combo is completed.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void ResetCombo();
	virtual void ResetCombo_Implementation();
};
