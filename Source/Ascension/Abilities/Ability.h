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
	
public:
	FString AbilityName;

public:
	/**
	 * Used to initialize the ability.
	 */
	virtual void Initialize();

	/**
	 * This method activates the ability.
	 */
	virtual void Activate();

	/**
	 * Called to update the ability every frame.
	 */
	virtual void Tick();

	/**
	 * This method finishes the execution of ability.
	 */
	virtual void Finish();
};
