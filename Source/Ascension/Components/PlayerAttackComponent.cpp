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

	else if (AttackType.Equals(FString("Upper Attack")))
	{
		AttackName = FString("UpperAttack");
	}

	return AttackName;
}

void UPlayerAttackComponent::SetupAttack_Implementation(const FString& AttackName, const uint8 AttackID)
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
		uint8 AttackID = 0;
		bool Activated = AbilitySystem->ActivateAbility(PlayerAttackName, AttackID);

		if (Activated)
		{
			ActiveAttackIDs.Add(AttackID);

			if (!ActiveAttackNameIDsMap.Contains(PlayerAttackName))
			{
				TArray<uint8> AttackIDs = TArray<uint8>();
				ActiveAttackNameIDsMap.Add(PlayerAttackName, AttackIDs);
			}
			ActiveAttackNameIDsMap[PlayerAttackName].Add(AttackID);

			ComboMeter = (++ComboMeter) % MaxComboCount;
			return true;
		}
	}
	
	return false;
}

void UPlayerAttackComponent::FinishAttack_Implementation(const FString& AttackName, const uint8 AttackID)
{
	UGameAbilitySystemComponent* AbilitySystem = Owner->FindComponentByClass<UGameAbilitySystemComponent>();

	if (!AttackName.Equals(FString("")))
	{
		if (ActiveAttackNameIDsMap.Contains(AttackName))
		{
			TArray<uint8> IDs = ActiveAttackNameIDsMap[AttackName];

			if (IDs.Num() > 0)
			{
				if (IDs.Contains(AttackID) && ActiveAttackIDs.Contains(AttackID))
				{
					if (AbilitySystem)
					{
						AbilitySystem->FinishAbility(AttackName, AttackID);
						ActiveAttackIDs.Remove(AttackID);
						ActiveAttackNameIDsMap[AttackName].Remove(AttackID);
					}
				}
				else if (ActiveAttackIDs.Contains(IDs[0]))
				{
					if (AbilitySystem)
					{
						AbilitySystem->FinishAbility(AttackName, IDs[0]);
						ActiveAttackIDs.Remove(IDs[0]);
						ActiveAttackNameIDsMap[AttackName].Remove(IDs[0]);
					}
				}
			}

			UPlayerStateComponent* StateComponent = Owner->FindComponentByClass<UPlayerStateComponent>();
			if (StateComponent)
			{
				StateComponent->SetCharacterState(ECharacterState::CS_Idle);
			}
		}
	}

	else if (ActiveAttackIDs.Contains(AttackID))
	{
		if (AbilitySystem)
		{
			AbilitySystem->FinishAbility(AttackName, AttackID);
			ActiveAttackIDs.Remove(AttackID);

			if (ActiveAttackNameIDsMap.Contains(AttackName))
			{
				if (ActiveAttackNameIDsMap[AttackName].Contains(AttackID))
				{
					ActiveAttackNameIDsMap[AttackName].Remove(AttackID);
				}
			}
		}

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
