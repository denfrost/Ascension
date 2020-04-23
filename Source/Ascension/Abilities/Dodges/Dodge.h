// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Ability.h"
#include "Dodge.generated.h"

/*
 * Class for implementing dodges which can be performed by entities.
 */
UCLASS()
class ASCENSION_API UDodge : public UAbility
{
	GENERATED_BODY()
	
public:
	/*
	 * Constructor of the dodge.
	 */
	UDodge();

	/*
	 * Method to activate the dodge.
	 */
	virtual void Activate();

protected:
	/** Animation montage of the dodge. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Variables")
	UAnimMontage* AnimMontage;

};
