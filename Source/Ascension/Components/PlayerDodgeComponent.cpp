// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "Abilities/AbilitySystems/GameAbilitySystemComponent.h"
#include "Components/PlayerStateComponent.h"
#include "PlayerDodgeComponent.h"


FString UPlayerDodgeComponent::SelectDodge_Implementation(const FString& DodgeType)
{
	// TODO: Implement direction and lock-on based dodges/steps.
	return DodgeType;
}

void UPlayerDodgeComponent::SetupDodge_Implementation(const FString& DodgeName = FString("Dodge"), const uint8 DodgeID = 0)
{
	UPlayerStateComponent* StateComponent = Owner->FindComponentByClass<UPlayerStateComponent>();
	if (StateComponent != nullptr)
	{
		StateComponent->SetCharacterState(ECharacterState::CS_Dodging);
	}
}

bool UPlayerDodgeComponent::Dodge_Implementation(const FString& DodgeName = FString("Dodge"))
{
	// This is done to choose the correct dodge.
	FString PlayerDodgeName = SelectDodge(DodgeName);

	return Super::Dodge_Implementation(PlayerDodgeName);
}

void UPlayerDodgeComponent::FinishDodge_Implementation(const FString& DodgeName = FString("Dodge"), const uint8 DodgeID = 0)
{
	UGameAbilitySystemComponent* AbilitySystem = Owner->FindComponentByClass<UGameAbilitySystemComponent>();

	if (!DodgeName.Equals(FString("")))
	{
		if (ActiveDodgeNameIDsMap.Contains(DodgeName))
		{
			TArray<uint8> IDs = ActiveDodgeNameIDsMap[DodgeName];

			if (IDs.Num() > 0)
			{
				if (IDs.Contains(DodgeID) && ActiveDodgeIDs.Contains(DodgeID))
				{
					if (AbilitySystem)
					{
						AbilitySystem->FinishAbility(DodgeName, DodgeID);
						ActiveDodgeIDs.Remove(DodgeID);
						ActiveDodgeNameIDsMap[DodgeName].Remove(DodgeID);
					}
				}
				else if (ActiveDodgeIDs.Contains(IDs[0]))
				{
					if (AbilitySystem)
					{
						AbilitySystem->FinishAbility(DodgeName, IDs[0]);
						ActiveDodgeIDs.Remove(IDs[0]);
						ActiveDodgeNameIDsMap[DodgeName].Remove(IDs[0]);
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

	else if (ActiveDodgeIDs.Contains(DodgeID))
	{
		if (AbilitySystem)
		{
			AbilitySystem->FinishAbility(DodgeName, DodgeID);
			ActiveDodgeIDs.Remove(DodgeID);

			if (ActiveDodgeNameIDsMap.Contains(DodgeName))
			{
				if (ActiveDodgeNameIDsMap[DodgeName].Contains(DodgeID))
				{
					ActiveDodgeNameIDsMap[DodgeName].Remove(DodgeID);
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
