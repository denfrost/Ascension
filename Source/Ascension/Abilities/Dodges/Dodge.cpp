// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "Dodge.h"
#include "Abilities/AbilitySystems/GameAbilitySystemComponent.h"


UDodge::UDodge()
	: Super()
{
	AnimMontage = nullptr;
}

void UDodge::Activate()
{
	if (AnimMontage != nullptr && AbilitySystem != nullptr)
	{
		ACharacter* Owner = Cast<ACharacter>(AbilitySystem->GetOwner());
		Owner->PlayAnimMontage(AnimMontage);
	}
}
