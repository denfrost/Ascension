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

void UGameAbilitySystemComponent::ActivateAbility(const FString& AbilityName, bool& Activated = false, uint32& AbilityID = 0)
{
	TSubclassOf<UAbility> AbilityClass = GetAbility(AbilityName);

	AbilityID = 0;
	Activated = false;

	if (AbilityClass != nullptr)
	{
		if (CanActivateAbility(AbilityName))
		{
			UE_LOG(LogTemp, Warning, TEXT("Activating ability."))
			UAbility* Ability = NewObject<*AbilityClass>(AbilityName, this);
			SetupAbility(AbilityName);
			Ability->Activate();
			AbilityID = Ability->GetUniqueID();
			ActiveAbilitiesMap.Add(AbilityID, Ability);
			Activated = true;
		}
	}
}

void UGameAbilitySystemComponent::EndAbility(const uint32& AbilityID = 0)
{
}

void UGameAbilitySystemComponent::FinishAbility(const uint32& AbilityID = 0)
{
	if (ActiveAbilitiesMap.Contains(AbilityName))
	{
		UE_LOG(LogTemp, Warning, TEXT("Finishing ability."))
		ActiveAbilitiesMap[AbilityID]->Finish();
		EndAbility(AbilityID);
		ActiveAbilitiesMap.Remove(AbilityID);
	}
}
