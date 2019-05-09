// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "Ability.h"
#include "AbilitySystems/GameAbilitySystemComponent.h"


UAbility::UAbility()
{
	AbilitySystem = nullptr;
}

void UAbility::Initialize(UGameAbilitySystemComponent* System)
{
	AbilitySystem = System;
}

bool UAbility::CanActivate() const
{
	return false;
}

void UAbility::Activate() {}

void UAbility::Finish() {}
