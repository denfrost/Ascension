// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "PlayerAttackComponent.h"
#include "Entities/Characters/Player/AscensionCharacter.h"
#include "Abilities/AbilitySystems/GameAbilitySystemComponent.h"


UPlayerAttackComponent::UPlayerAttackComponent()
{
	CanChain = false;
	ComboMeter = 0;
	MaxComboCount = 3;
}

void UPlayerAttackComponent::BeginPlay()
{
	Super::BeginPlay();
}

FString UPlayerAttackComponent::SelectAttack_Implementation(const FString& AttackType)
{
	FString AttackName = FString();

	// ToDo: Don't hard-code ability names.
	if (AbilitySystem)
	{
		if (AttackType.Equals(FString("Light Attack")))
		{
			switch (ComboMeter)
			{
			case 0:
				AttackName = FString("Light01");
				break;

			case 1:
				AttackName = FString("Light02");
				break;

			case 2:
				AttackName = FString("Light03");
				break;

			default:
				AttackName = FString("Light01");
				break;
			}
		}

		else if (AttackType.Equals(FString("Strong Attack")))
		{
			switch (ComboMeter)
			{
			case 0:
				AttackName = FString("Strong01");
				break;

			case 1:
				AttackName = FString("Strong02");
				break;

			case 2:
				AttackName = FString("Strong03");
				break;

			default:
				AttackName = FString("Strong01");
				break;
			}
		}
	}

	return Attack;
}

void UPlayerAttackComponent::Attack_Implementation(const FString& AttackName, const FVector& MovementIntent)
{
	// This is done to choose the correct attack in a combo.
	FString PlayerAttackName = SelectAttack(AttackName);

	UGameAbilitySystemComponent* AbilitySystem = Owner->FindComponentByClass<UGameAbilitySystemComponent>();
	if (AbilitySystem->CanActivateAbility(AttackName))
	{
		if (!ActiveAttack.IsEmpty())
		{
			AbilitySystem->FinishAbility(ActiveAttack);
		}

		ActionDirection = MovementIntent;
		AbilitySystem->ActivateAbility(AttackName);
		ActiveAttack = Attack->AbilityName;
		ComboMeter = (++ComboMeter) % MaxComboCount;
	}
}

void UPlayerAttackComponent::ResetCombo_Implementation()
{
	ComboMeter = 0;
}