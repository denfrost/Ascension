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
	ActiveDodgeIDs.Empty();
	ActiveDodgeNameIDsMap.Empty();
}

// Called when the game starts
void UDodgeComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ACharacter>(GetOwner());
}

void UDodgeComponent::SetupDodge_Implementation(const FString& DodgeName = FString("Dodge"), const uint8 DodgeID = 0) {}

bool UDodgeComponent::Dodge_Implementation(const FString& DodgeName = FString("Dodge"))
{
	UGameAbilitySystemComponent* AbilitySystem = Owner->FindComponentByClass<UGameAbilitySystemComponent>();

	if (AbilitySystem)
	{
		if (AbilitySystem->CanActivateAbility(DodgeName))
		{
			uint8 DodgeID = 0;
			bool Activated = AbilitySystem->ActivateAbility(DodgeName, DodgeID);

			if (Activated)
			{
				ActiveDodgeIDs.Add(DodgeID);

				if (!ActiveDodgeNameIDsMap.Contains(DodgeName))
				{
					TArray<uint8> DodgeIDs = TArray<uint8>();
					ActiveDodgeNameIDsMap.Add(DodgeName, DodgeIDs);
				}
				ActiveDodgeNameIDsMap[DodgeName].Add(DodgeID);

				return true;
			}
		}
	}

	return false;
}

void UDodgeComponent::FinishDodge_Implementation(const FString& DodgeName = FString("Dodge"), const uint8 DodgeID = 0)
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
	}
}
