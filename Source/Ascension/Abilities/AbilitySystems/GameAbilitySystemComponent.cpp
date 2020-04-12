// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "GameAbilitySystemComponent.h"
#include "UObject/UObjectGlobals.h"
#include "Abilities/Ability.h"

// Sets default values for this component's properties
UGameAbilitySystemComponent::UGameAbilitySystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ClearAbilities();
	Owner = GetOwner();
}


// Called when the game starts
void UGameAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UGameAbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

TSubclassOf<UAbility> UGameAbilitySystemComponent::GetAbility(const FString& AbilityName) const
{
	if (AbilitiesMap.Contains(AbilityName))
	{
		return AbilitiesMap[AbilityName];
	}
	
	return nullptr;
}

UAbility* UGameAbilitySystemComponent::GetActiveAbility(const FString& AbilityName) const
{
	if (ActiveAbilitiesMap.Contains(AbilityName))
	{
		return ActiveAbilitiesMap[AbilityName];
	}

	return nullptr;
}

void UGameAbilitySystemComponent::AddAbility(const FString& AbilityName, TSubclassOf<UAbility> Ability)
{
	if (!AbilitiesMap.Contains(AbilityName))
	{
		AbilitiesMap.Add(AbilityName, Ability);
	}
}

void UGameAbilitySystemComponent::ClearAbilities()
{
	AbilitiesMap.Empty();
	ActiveAbilitiesMap.Empty();
}

bool UGameAbilitySystemComponent::CanActivateAbility(const FString& AbilityName)
{
	return true;
}

void UGameAbilitySystemComponent::SetupAbility(const FString& AbilityName)
{
}

bool UGameAbilitySystemComponent::ActivateAbility(const FString& AbilityName)
{
	TSubclassOf<UAbility> AbilityClass = GetAbility(AbilityName);

	if (AbilityClass != nullptr)
	{
		if (CanActivateAbility(AbilityName))
		{
			UAbility* Ability = NewObject<UAbility>(this, AbilityClass);
			Ability->Initialize(AbilityName, this);
			SetupAbility(AbilityName);
			Ability->Activate();
			ActiveAbilitiesMap.Add(AbilityName, Ability);
			return true;
		}
	}

	return false;
}

void UGameAbilitySystemComponent::EndAbility(const FString& AbilityName)
{
}

void UGameAbilitySystemComponent::FinishAbility(const FString& AbilityName)
{
	if (ActiveAbilitiesMap.Contains(AbilityName))
	{
		ActiveAbilitiesMap[AbilityName]->Finish();
		EndAbility(AbilityName);
		ActiveAbilitiesMap.Remove(AbilityName);
	}
}
