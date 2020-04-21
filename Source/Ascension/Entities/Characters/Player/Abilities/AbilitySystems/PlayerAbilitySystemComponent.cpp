// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "Entities/Characters/Player/AscensionCharacter.h"
#include "Abilities/Ability.h"
#include "PlayerAbilitySystemComponent.h"


// Sets default values for this component's properties
UPlayerAbilitySystemComponent::UPlayerAbilitySystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
}


bool UPlayerAbilitySystemComponent::CanActivateAbility(const FString& AbilityName)
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

void UPlayerAbilitySystemComponent::SetupAbility(const FString& AbilityName)
{
	if (IsAttack(AbilityName))
	{
		AAscensionCharacter* Player = Cast<AAscensionCharacter>(Owner);
		Player->SetCharacterState(ECharacterState::CS_Attacking);
		Player->DisableMovement();

		// This is being set to false here as the anim notify state for chaining can take some time to end.
		// TODO: Find a better way to do this or try to fix how anim notifies work.
		CanChain = false;
	}
}

void UPlayerAbilitySystemComponent::EndAbility(const FString& AbilityName)
{
	if (ActiveAbilitiesMap.Contains((AbilityName)))
	{
		TArray<FString> CurrentActiveAbilities;
		ActiveAbilitiesMap.GenerateKeyArray(CurrentActiveAbilities);
		int NumOtherActiveAttacks = 0;

		for (int i = 0; i < CurrentActiveAbilities.Num(); i++)
		{
			if (IsAttack(CurrentActiveAbilities[i]) && !(CurrentActiveAbilities[i].Equals(AbilityName)))
			{
				NumOtherActiveAttacks++;
			}
		}

		// TODO: Need a better way to figure out whether to transition state rather than just checking the number of active attacks.
		if (IsAttack(AbilityName) && !(NumOtherActiveAttacks > 0))
		{
			AAscensionCharacter* Player = Cast<AAscensionCharacter>(Owner);
			Player->SetCharacterState(ECharacterState::CS_Idle);
			Player->EnableMovement();
		}
	}
}

void UPlayerAbilitySystemComponent::SetCanChain(const bool& Chain)
{
	CanChain = Chain;
}

bool UPlayerAbilitySystemComponent::IsAttack(const FString& AbilityName)
{
	// TODO: Don't hard-code ability names.
	if (AbilityName.Equals("Light01") || AbilityName.Equals("Light02") || AbilityName.Equals("Light03") ||
		AbilityName.Equals("Strong01") || AbilityName.Equals("Strong02") || AbilityName.Equals("Strong03"))
	{
		return true;
	}

	return false;
}
