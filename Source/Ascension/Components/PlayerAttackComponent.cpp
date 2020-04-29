// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "PlayerAttackComponent.h"
#include "Components/PlayerStateComponent.h"
#include "Entities/Characters/Player/Abilities/AbilitySystems/PlayerAbilitySystemComponent.h"
#include "Entities/Characters/Player/AscensionCharacter.h"
#include "Abilities/AbilitySystems/GameAbilitySystemComponent.h"


UPlayerAttackComponent::UPlayerAttackComponent()
{
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

	return AttackName;
}

void UPlayerAttackComponent::SetupAttack_Implementation(const FString& AttackName)
{
	UPlayerStateComponent* StateComponent = Owner->FindComponentByClass<UPlayerStateComponent>();
	UPlayerAbilitySystemComponent* AbilitySystemComponent = Owner->FindComponentByClass<UPlayerAbilitySystemComponent>();
	if (StateComponent != nullptr && AbilitySystemComponent != nullptr)
	{
		StateComponent->SetCharacterState(ECharacterState::CS_Attacking);
		AbilitySystemComponent->SetCanChain(false);
	}
}

bool UPlayerAttackComponent::Attack_Implementation(const FString& AttackName)
{
	// This is done to choose the correct attack in a combo.
	FString PlayerAttackName = SelectAttack(AttackName);

	UGameAbilitySystemComponent* AbilitySystem = Owner->FindComponentByClass<UGameAbilitySystemComponent>();
	if (AbilitySystem->CanActivateAbility(PlayerAttackName))
	{
		if (ActiveAttacks.Contains(PlayerAttackName))
		{
			AbilitySystem->FinishAbility(PlayerAttackName);
		}
		bool Activated = AbilitySystem->ActivateAbility(PlayerAttackName);
		if (Activated)
		{
			ActiveAttacks.Add(PlayerAttackName);
			ComboMeter = (++ComboMeter) % MaxComboCount;
			return true;
		}
	}
	
	return false;
}

void UPlayerAttackComponent::FinishAttack_Implementation(const FString& AttackName)
{
	if (ActiveAttacks.Contains(AttackName))
	{
		ActiveAttacks.Remove(AttackName);

		UPlayerStateComponent* StateComponent = Owner->FindComponentByClass<UPlayerStateComponent>();
		if (StateComponent)
		{
			StateComponent->SetCharacterState(ECharacterState::CS_Idle);
		}
	}
}

void UPlayerAttackComponent::ResetCombo_Implementation()
{
	ComboMeter = 0;
}