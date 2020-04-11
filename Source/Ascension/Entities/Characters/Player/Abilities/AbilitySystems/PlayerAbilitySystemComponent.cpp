// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "Entities/Characters/Player/AscensionCharacter.h"
#include "PlayerAbilitySystemComponent.h"


bool UGameAbilitySystemComponent::CanActivateAbility(const FString& AbilityName)
{
	const AAscensionCharacter* Player = Cast<AAscensionCharacter>(Owner);

	if ((GetAbility(AbilityName) != nullptr) && (IsAttack(AbilityName)))
	{
		if ((Player->GetCharacterState() == ECharacterState::CS_Idle ||
			 Player->GetCharacterState() == ECharacterState::CS_Attacking) &&
			(Player->GetMovementState() == EMovementState::MS_OnGround) &&
			(Player->GetWeaponState() == EWeaponState::WS_Unsheathed) &&
			!Player->Dead)
		{
			if (Player->GetCharacterState() == ECharacterState::CS_Attacking)
			{
				if (CanChain)
				{
					// TODO: This needs to be done by a anim notify sequence and not via a variable.
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

void UGameAbilitySystemComponent::SetupAbility(const FString& AbilityName)
{
	UE_LOG(LogTemp, Warning, TEXT("Setting up ability."))
	if (IsAttack(AbilityName))
	{
		UE_LOG(LogTemp, Warning, TEXT("Setting up attack."))
		AAscensionCharacter* Player = Cast<AAscensionCharacter>(Owner);
		Player->SetCharacterState(ECharacterState::CS_Attacking);
		Player->DisableMovement();
	}
}

void UGameAbilitySystemComponent::EndAbility(const uint32& AbilityID)
{
	if (ActiveAbilitiesMap.Contains((AbilityID)))
	{
		UE_LOG(LogTemp, Warning, TEXT("Ending ability."))
		if (IsAttack(ActiveAbilitiesMap[AbilityID]->AbilityName))
		{
			UE_LOG(LogTemp, Warning, TEXT("Ending attack."))
			AAscensionCharacter* Player = Cast<AAscensionCharacter>(Owner);
			Player->SetCharacterState(ECharacterState::CS_Idle);
			Player->EnableMovement();
		}
	}
}

void UPlayerAttackComponent::SetCanChain(const bool& Chain)
{
	CanChain = Chain;
}

bool UGameAbilitySystemComponent::IsAttack(const FString& AbilityName)
{
	// TODO: Don't hard-code ability names.
	if (AbilityName.Equals("Light01") || AbilityName.Equals("Light02") || AbilityName.Equals("Light03") ||
		AbilityName.Equals("Strong01") || AbilityName.Equals("Strong02") || AbilityName.Equals("Strong03"))
	{
		return true;
	}

	return false;
}
