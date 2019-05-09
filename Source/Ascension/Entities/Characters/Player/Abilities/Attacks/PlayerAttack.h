// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Attacks/Attack.h"
#include "PlayerAttack.generated.h"

/**
 * 
 */
UCLASS()
class ASCENSION_API UPlayerAttack : public UAttack
{
	GENERATED_BODY()
	
public:
	virtual bool CanActivate() const override;
};
