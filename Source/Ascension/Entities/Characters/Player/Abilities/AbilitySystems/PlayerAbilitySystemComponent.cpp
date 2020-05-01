// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "Components/PlayerStateComponent.h"
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
	UPlayerStateComponent* StateComponent = Player->FindComponentByClass<UPlayerStateComponent>();

	if (StateComponent)
	{
		if (GetAbility(AbilityName) != nullptr)
		{
			if (IsAttack(AbilityName))
			{
				if ((StateComponent->GetCharacterState() == ECharacterState::CS_Idle ||
					 StateComponent->GetCharacterState() == ECharacterState::CS_Attacking ||
					 StateComponent->GetCharacterState() == ECharacterState::CS_Dodging) &&
					(StateComponent->GetMovementState() == EMovementState::MS_OnGround) &&
					(StateComponent->GetWeaponState() == EWeaponState::WS_Unsheathed) &&
					!Player->Dead)
				{
					if (StateComponent->GetCharacterState() == ECharacterState::CS_Attacking ||
						StateComponent->GetCharacterState() == ECharacterState::CS_Dodging)
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

			else if (IsDodge(AbilityName))
			{
				if ((StateComponent->GetCharacterState() == ECharacterState::CS_Idle ||
					 StateComponent->GetCharacterState() == ECharacterState::CS_Attacking ||
					 StateComponent->GetCharacterState() == ECharacterState::CS_Dodging) &&
					(StateComponent->GetMovementState() == EMovementState::MS_OnGround) &&
					!Player->Dead)
				{
					if (StateComponent->GetCharacterState() == ECharacterState::CS_Attacking ||
						StateComponent->GetCharacterState() == ECharacterState::CS_Dodging)
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
			}
		}
	}
	
	return false;
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

bool UPlayerAbilitySystemComponent::IsDodge(const FString& AbilityName)
{
	// TODO: Don't hard-code ability names.
	if (AbilityName.Equals("Dodge"))
	{
		return true;
	}

	return false;
}
