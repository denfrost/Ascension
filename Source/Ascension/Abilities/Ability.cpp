// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "Ability.h"
#include "AbilitySystems/GameAbilitySystemComponent.h"


UAbility::UAbility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystem = nullptr;
}

void UAbility::Initialize(UGameAbilitySystemComponent* System)
{
	AbilitySystem = System;
}

void UAbility::Activate() {}

void UAbility::Finish() {}
