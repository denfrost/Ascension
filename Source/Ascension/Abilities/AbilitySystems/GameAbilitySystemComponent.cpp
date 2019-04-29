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
	ActiveAbility = nullptr;
	Owner = nullptr;
}


// Called when the game starts
void UGameAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Owner = Cast<AActor>(GetOwner());
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
		Ability->Initialize();
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

bool UGameAbilitySystemComponent::CanActivateAbility()
{
	return false;
}

void UGameAbilitySystemComponent::ActivateAbility(FString AbilityName)
{
	UAbility* Ability = GetAbility(AbilityName);

	if (Ability != nullptr)
	{
		Ability->Activate();
	}
}

void UGameAbilitySystemComponent::FinishAbility() {}