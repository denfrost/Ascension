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
	 * Function to check whether the player can attack.
	 * @param Chain		Whether attacks can be chained.
	 */
	UFUNCTION(BlueprintCallable, Category = "Attacks")
	void SetCanChain(const bool& Chain);

protected:
	/*
	 * This method checks whether an ability is an attack.
	 * TODO: Instead of a bool return type, use enums.
	 * @param AbilityName	Name of ability to check.
	 * @returns bool		Whether the ability is an attack.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Abilities")
	virtual bool IsAttack(const FString& AbilityName);

	/*
	 * This method checks whether an ability is a dodge.
	 * TODO: Instead of a bool return type, use enums.
	 * @param AbilityName	Name of ability to check.
	 * @returns bool		Whether the ability is a dodge.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Abilities")
	virtual bool IsDodge(const FString& AbilityName);
	
};
