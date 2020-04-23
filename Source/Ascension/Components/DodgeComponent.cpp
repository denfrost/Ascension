// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "Abilities/AbilitySystems/GameAbilitySystemComponent.h"
#include "DodgeComponent.h"


// Sets default values for this component's properties
UDodgeComponent::UDodgeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame. You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Clear active dodges.
	ActiveDodges.Empty();
}

// Called when the game starts
void UDodgeComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ACharacter>(GetOwner());
}

bool UDodgeComponent::Dodge_Implementation(const FString& DodgeName = FString("Dodge"))
{
	UGameAbilitySystemComponent* AbilitySystem = Owner->FindComponentByClass<UGameAbilitySystemComponent>();

	if (AbilitySystem)
	{
		if (AbilitySystem->CanActivateAbility(DodgeName))
		{
			if (ActiveDodges.Contains(DodgeName))
			{
				AbilitySystem->FinishAbility(DodgeName);
			}
			bool Activated = AbilitySystem->ActivateAbility(DodgeName);
			if (Activated)
			{
				ActiveDodges.Add(DodgeName);
				return true;
			}
		}
	}

	return false;
}

void UDodgeComponent::FinishDodge_Implementation(const FString& DodgeName = FString("Dodge"))
{
	UGameAbilitySystemComponent* AbilitySystem = Owner->FindComponentByClass<UGameAbilitySystemComponent>();

	if (AbilitySystem)
	{
		if (ActiveDodges.Contains(DodgeName))
		{
			ActiveDodges.Remove(DodgeName);
			AbilitySystem->FinishAbility(DodgeName);
		}
	}
}
