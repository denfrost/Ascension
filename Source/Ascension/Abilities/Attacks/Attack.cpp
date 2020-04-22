// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "Attack.h"
#include "Abilities/AbilitySystems/GameAbilitySystemComponent.h"

UAttack::UAttack()
	: Super()
{
	AnimMontage = nullptr;
}

void UAttack::Activate()
{
	if (AnimMontage != nullptr && AbilitySystem != nullptr)
	{
		ACharacter* Owner = Cast<ACharacter>(AbilitySystem->GetOwner());
		UE_LOG(LogTemp, Warning, TEXT("Starting montage."))
		Owner->PlayAnimMontage(AnimMontage);
	}
}

FAttackEffectInfo UAttack::GetEffectInfo() const
{
	return EffectInfo;
}
