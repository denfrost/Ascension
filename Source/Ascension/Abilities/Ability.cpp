// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "Ability.h"
#include "AbilitySystems/GameAbilitySystemComponent.h"


UAbility::UAbility()
{
	AbilityName = FString("Ability");
	AbilitySystem = nullptr;
}

void UAbility::Initialize(FString Name, UGameAbilitySystemComponent* System = nullptr)
{
	AbilityName = Name;
	AbilitySystem = System;
}

void UAbility::Activate() {}

void UAbility::Finish() {}

FCustomMovementParams UAbility::GetMovementParams_Implementation() const
{
	return MovementParams;
}
