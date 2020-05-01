// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "Ability.h"
#include "AbilitySystems/GameAbilitySystemComponent.h"


UAbility::UAbility()
{
	AbilityName = FString("Ability");
	AbilityID = 0;
	AbilitySystem = nullptr;
}

void UAbility::Initialize(FString Name, uint8 ID, UGameAbilitySystemComponent* System = nullptr)
{
	AbilityName = Name;
	AbilityID = ID;
	AbilitySystem = System;
}

void UAbility::Activate() {}

void UAbility::Finish() {}

FCustomMovementParams UAbility::GetMovementParams_Implementation() const
{
	return MovementParams;
}
