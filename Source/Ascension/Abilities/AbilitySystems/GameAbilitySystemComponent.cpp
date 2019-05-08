// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "GameAbilitySystemComponent.h"
#include "Abilities/Ability.h"

// Sets default values for this component's properties
UGameAbilitySystemComponent::UGameAbilitySystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Abilities.Empty();
	ActiveAbilities.Empty();
	Owner = nullptr;
}


// Called when the game starts
void UGameAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Owner = Cast<AActor>(GetOwner());

	for (int i = 0; i < Abilities.Num(); i++)
	{
		Abilities[i]->Initialize(this);
	}
}


// Called every frame
void UGameAbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGameAbilitySystemComponent::InitializeAbility(FString AbilityName)
{
	UAbility* Ability = GetAbility(AbilityName);

	if (Ability != nullptr)
	{
		Ability->Initialize(this);
	}
}

UAbility* UGameAbilitySystemComponent::GetAbility(FString AbilityName)
{
	for (int i = 0; i < Abilities.Num(); i++)
	{
		if (Abilities[i]->AbilityName == AbilityName)
		{
			return Abilities[i];
		}
	}

	return nullptr;
}

void UGameAbilitySystemComponent::AddAbility(UAbility* Ability)
{
	UAbility* ExistingAbility = GetAbility(Ability->AbilityName);
	
	if (ExistingAbility == nullptr)
	{
		Abilities.Add(Ability);
		Ability->Initialize(this);
	}
}

bool UGameAbilitySystemComponent::CanActivateAbility(const UAbility* Ability)
{
	if (Ability != nullptr)
	{
		return Ability->CanActivate();
	}

	return false;
}

void UGameAbilitySystemComponent::ActivateAbility(FString AbilityName)
{
	UAbility* Ability = GetAbility(AbilityName);

	if (Ability != nullptr)
	{
		if (CanActivateAbility(Ability))
		{
			Ability->Activate();
			ActiveAbilities.Add(Ability);
		}
	}
}

void UGameAbilitySystemComponent::FinishAbility(FString AbilityName)
{
	for (int i = 0; i < ActiveAbilities.Num(); i++)
	{
		if (ActiveAbilities[i]->AbilityName.Equals(AbilityName))
		{
			ActiveAbilities[i]->Finish();
			ActiveAbilities.RemoveAt(i);
			i--;
		}
	}
}