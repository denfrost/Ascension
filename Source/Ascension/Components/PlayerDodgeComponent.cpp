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

void UPlayerDodgeComponent::SetupDodge_Implementation(const FString& DodgeName = FString("Dodge"))
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

	UGameAbilitySystemComponent* AbilitySystem = Owner->FindComponentByClass<UGameAbilitySystemComponent>();
	if (AbilitySystem->CanActivateAbility(PlayerDodgeName))
	{
		if (ActiveDodges.Contains(PlayerDodgeName))
		{
			AbilitySystem->FinishAbility(PlayerDodgeName);
		}
		bool Activated = AbilitySystem->ActivateAbility(PlayerDodgeName);
		if (Activated)
		{
			ActiveDodges.Add(PlayerDodgeName);
			return true;
		}
	}

	return false;
}

void UPlayerDodgeComponent::FinishDodge_Implementation(const FString& DodgeName = FString("Dodge"))
{
	if (ActiveDodges.Contains(DodgeName))
	{
		ActiveDodges.Remove(DodgeName);

		UPlayerStateComponent* StateComponent = Owner->FindComponentByClass<UPlayerStateComponent>();
		if (StateComponent != nullptr)
		{
			StateComponent->SetCharacterState(ECharacterState::CS_Idle);
		}
	}
}
