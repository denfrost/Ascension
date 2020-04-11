// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "Attack.h"
#include "Abilities/AbilitySystems/GameAbilitySystemComponent.h"

UAttack::UAttack(FString Name, UGameAbilitySystemComponent* System = nullptr)
	: Super(Name, System)
{
	AnimMontage = nullptr;
	FVector AttackDirection = FVector();
}

void UAttack::Activate(FVector AttackDirection)
{
	if (AnimMontage != nullptr && AbilitySystem != nullptr)
	{
		ACharacter* Owner = Cast<ACharacter>(AbilitySystem->GetOwner());
		this->AttackDirection = AttackDirection;
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
