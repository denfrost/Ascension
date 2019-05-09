// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "Attack.h"
#include "Abilities/AbilitySystems/GameAbilitySystemComponent.h"

UAttack::UAttack()
{
	AnimMontage = nullptr;
}

void UAttack::Activate()
{
	if (AnimMontage != nullptr && AbilitySystem != nullptr)
	{
		ACharacter* Owner = Cast<ACharacter>(AbilitySystem->GetOwner());
		Owner->PlayAnimMontage(AnimMontage);
	}
}

FAttackMovementInfo UAttack::GetMovementInfo() const
{
	return MovementInfo;
}

FAttackEffectInfo UAttack::GetEffectInfo() const
{
	return EffectInfo;
}
