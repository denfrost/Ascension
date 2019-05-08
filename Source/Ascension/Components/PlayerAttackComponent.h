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
	
protected:
	/** Set to true if the character can chain an attack. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Gameplay")
	bool CanChain;

public:
	bool CanAttack();

	/** Function to check whether the player can attack. */
	UFUNCTION(BlueprintCallable, Category = "Setters")
	void SetCanChain(bool Chain);

	/** Implementation for selecting attacks. */
	virtual UAttack* SelectAttack_Implementation(const FString& AttackType);

	/** Implementation for resetting attacks. */
	virtual void Reset_Implementation();
};
