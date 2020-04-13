// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/AbilitySystems/GameAbilitySystemComponent.h"
#include "PlayerAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class ASCENSION_API UPlayerAbilitySystemComponent : public UGameAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	UPlayerAbilitySystemComponent();

protected:
	/** Set to true if the character can chain an attack. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Gameplay")
	bool CanChain;

public:
	/*
	 * Function to check whether the entity can use an ability.
	 * @param AbilityName	Name of ability to check for.
	 */
	virtual bool CanActivateAbility(const FString& AbilityName);

	/*
	 * This method does the setup necessary to activate an ability such as state transitions.
	 * @param AbilityName	Name of the ability to setup.
	 */
	virtual void SetupAbility(const FString& AbilityName);

	/*
	 * This method does the actions necessary to finish an ability such as state transitions.
	 * @param AbilityName	Name of the ability to end.
	 */
	virtual void EndAbility(const FString& AbilityName);

	/** Function to check whether the player can attack. */
	void SetCanChain(const bool& Chain);

protected:
	/*
	 * This method checks whether an ability is an attack.
	 * TODO: Instead of a bool return type, use enums.
	 * @param AbilityName	Name of ability to check.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Abilities")
	virtual bool IsAttack(const FString& AbilityName);
	
};
