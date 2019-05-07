// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "PlayerAttackComponent.h"
#include "Entities/Characters/Player/AscensionCharacter.h"
#include "Abilities/AbilitySystems/GameAbilitySystemComponent.h"


UPlayerAttackComponent::UPlayerAttackComponent()
{
	CanChain = false;
}

void UPlayerAttackComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UPlayerAttackComponent::CanAttack()
{
	const AAscensionCharacter* Player = Cast<AAscensionCharacter>(Owner);

	if (Player)
	{
		if ((Player->GetCharacterState() == ECharacterState::CS_Idle || Player->GetCharacterState() == ECharacterState::CS_Attacking) &&
			(Player->GetMovementState() == EMovementState::MS_OnGround) && (Player->GetWeaponState() == EWeaponState::WS_Unsheathed) && !Player->Dead)
		{
			if (Player->GetCharacterState() == ECharacterState::CS_Attacking)
			{
				if (CanChain)
				{
					CanChain = false;
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return true;
			}
		}
		else
		{
			return false;
		}
	}

	return false;
}

void UPlayerAttackComponent::SetCanChain(bool Chain)
{
	CanChain = Chain;
}

UAttack* UPlayerAttackComponent::SelectAttack_Implementation(const FString& AttackType)
{
	UGameAbilitySystemComponent* AbilitySystem = Owner->FindComponentByClass<UGameAbilitySystemComponent>();
	UAttack* Attack = nullptr;

	if (AbilitySystem)
	{
		if (AttackType.Equals(FString("Light Attack")))
		{
			switch (ComboMeter)
			{
			case 0:
				Attack = Cast<UAttack>(AbilitySystem->GetAbility(FString("Light01")));
				ComboMeter++;
				break;

			case 1:
				Attack = Cast<UAttack>(AbilitySystem->GetAbility(FString("Light02")));
				ComboMeter++;
				break;

			case 2:
				Attack = Cast<UAttack>(AbilitySystem->GetAbility(FString("Light03")));
				ComboMeter++;
				break;

			default:
				Attack = Cast<UAttack>(AbilitySystem->GetAbility(FString("Light01")));
				ComboMeter = 1;
				break;
			}
		}

		else if (AttackType.Equals(FString("Strong Attack")))
		{
			switch (ComboMeter)
			{
			case 0:
				Attack = Cast<UAttack>(AbilitySystem->GetAbility(FString("Strong01")));
				ComboMeter++;
				break;

			case 1:
				Attack = Cast<UAttack>(AbilitySystem->GetAbility(FString("Strong02")));
				ComboMeter++;
				break;

			case 2:
				Attack = Cast<UAttack>(AbilitySystem->GetAbility(FString("Strong03")));
				ComboMeter++;
				break;

			default:
				Attack = Cast<UAttack>(AbilitySystem->GetAbility(FString("Strong01")));
				ComboMeter = 1;
				break;
			}
		}
	}

	return Attack;

	/*
	bool Found = false;
	FAttackStruct Attack;
	

	if (AttackType.Equals(FString("Light Attack")))
	{
		switch (ComboMeter)
		{
		case 0:
			GetAttack(FString("Light01"), Found, Attack);
			ComboMeter++;
			break;

		case 1:
			GetAttack(FString("Light02"), Found, Attack);
			ComboMeter++;
			break;

		case 2:
			GetAttack(FString("Light03"), Found, Attack);
			ComboMeter++;
			break;

		default:
			GetAttack(FString("Light01"), Found, Attack);
			ComboMeter = 1;
			break;
		}
	}

	else if (AttackType.Equals(FString("Strong Attack")))
	{
		switch (ComboMeter)
		{
		case 0:
			GetAttack(FString("Strong01"), Found, Attack);
			ComboMeter++;
			break;

		case 1:
			GetAttack(FString("Strong02"), Found, Attack);
			ComboMeter++;
			break;

		case 2:
			GetAttack(FString("Strong03"), Found, Attack);
			ComboMeter++;
			break;

		default:
			GetAttack(FString("Strong01"), Found, Attack);
			ComboMeter = 1;
			break;
		}
	}

	SetAttack(Found, Attack);
	*/
}

void UPlayerAttackComponent::Reset_Implementation()
{
	Super::Reset_Implementation();
	CanChain = false;
	ComboMeter = 0;
}