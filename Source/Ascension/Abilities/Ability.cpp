// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "Ability.h"
#include "AbilitySystems/GameAbilitySystemComponent.h"


UAbility::UAbility(FString Name, UGameAbilitySystemComponent* System = nullptr)
{
	AbilityName = Name;
	AbilitySystem = System;
}

void UAbility::Initialize() {}

void UAbility::Activate() {}

void UAbility::Finish() {}
