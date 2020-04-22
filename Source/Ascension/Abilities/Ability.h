// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Globals.h"
#include "Ability.generated.h"

/*
 * Class for implementing abilities which can be performed by entities.
 */
UCLASS(Blueprintable, ClassGroup = (Abilities))
class ASCENSION_API UAbility : public UObject
{
	GENERATED_BODY()
	
public:
	/*
	 * Constructor of the ability.
	 */
	UAbility();

public:
	UPROPERTY(Category = Properties, EditDefaultsOnly, BlueprintReadWrite)
	FString AbilityName;

protected:
	/*
	 * Movement parameters for the ability.
	 * TODO: Think about refactoring this.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	FCustomMovementParams MovementParams;

public:
	/*
	 * Used to initialize the ability.
	 * @param Name		Name of the ability.
	 * @param System	Ability system component which handles this ability.
	 */
	UFUNCTION(BlueprintCallable, Category = "Interface Functions")
	virtual void Initialize(FString Name, class UGameAbilitySystemComponent* System);

	/*
	 * This method activates the ability.
	 */
	virtual void Activate();

	/*
	 * This method finishes the execution of ability.
	 */
	virtual void Finish();

	/*
	 * Function to get the movement direction of the ability.
	 * TODO: Think about refactoring this.
	 * @returns FCustomMovementParams	Movement parameters of the ability.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface Functions")
	FCustomMovementParams GetMovementParams() const;
	virtual FCustomMovementParams GetMovementParams_Implementation() const;

protected:
	// TODO: Find a better way to initialize this.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Properties)
	class UGameAbilitySystemComponent* AbilitySystem;
};
