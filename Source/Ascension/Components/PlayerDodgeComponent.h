// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/DodgeComponent.h"
#include "PlayerDodgeComponent.generated.h"

/**
 * 
 */
UCLASS()
class ASCENSION_API UPlayerDodgeComponent : public UDodgeComponent
{
	GENERATED_BODY()
	
public:
	/*
	 * Implementation for selecting dodges.
	 * @param DodgeType		Type of dodge to perform.
	 * @returns FString		Name of dodge ability to perform.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	FString SelectDodge(const FString& DodgeType);
	virtual FString SelectDodge_Implementation(const FString& DodgeType);

	/*
	 * Called for the character to perform the specified dodge.
	 * @param DodgeName		Name of the dodge to perform.
	 * @returns bool		Whether the dodge was executed.
	 */
	virtual bool Dodge_Implementation(const FString& DodgeName);

};
