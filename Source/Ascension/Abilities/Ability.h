// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Ability.generated.h"

/**
 * Class for implementing abilities which can be performed by entities.
 */
UCLASS(Blueprintable, ClassGroup = (Abilities))
class ASCENSION_API UAbility : public UObject
{
	GENERATED_BODY()
	
	/**
	 * Constructor of the ability.
	 */
	UAbility(const FObjectInitializer& ObjectInitializer);

public:
	FString AbilityName;

public:
	/**
	 * Used to initialize the ability.
	 */
	virtual void Initialize(class UGameAbilitySystemComponent* System);

	/** 
	 * Function to check whether an ability can be activated.
	 */
	virtual bool CanActivate() const = 0;

	/**
	 * This method activates the ability.
	 */
	virtual void Activate();

	/**
	 * This method finishes the execution of ability.
	 */
	virtual void Finish();

protected:
	// TODO: Find a better way to initialize this.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	class UGameAbilitySystemComponent* AbilitySystem;
};
