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
	NextAbilityID = 0;
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

UAbility* UGameAbilitySystemComponent::GetActiveAbility(const FString& AbilityName = FString(""), const uint8& AbilityID = 0) const
{
	if (!AbilityName.Equals(FString("")))
	{
		if (ActiveAbilityNameIDsMap.Contains(AbilityName))
		{
			TArray<uint8> IDs = ActiveAbilityNameIDsMap[AbilityName];

			if (IDs.Num() > 0)
			{
				if (IDs.Contains(AbilityID) && ActiveAbilitiesMap.Contains(AbilityID))
				{
					return ActiveAbilitiesMap[AbilityID];
				}
				else if (ActiveAbilitiesMap.Contains(IDs[0]))
				{
					return ActiveAbilitiesMap[IDs[0]];
				}
			}
		}
	}
	
	else if (ActiveAbilitiesMap.Contains(AbilityID))
	{
		return ActiveAbilitiesMap[AbilityID];
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
	ActiveAbilityNameIDsMap.Empty();
}

bool UGameAbilitySystemComponent::CanActivateAbility(const FString& AbilityName)
{
	return true;
}

bool UGameAbilitySystemComponent::ActivateAbility(const FString& AbilityName, uint8& ActivatedAbilityID)
{
	TSubclassOf<UAbility> AbilityClass = GetAbility(AbilityName);

	if (AbilityClass != nullptr)
	{
		if (CanActivateAbility(AbilityName))
		{
			UAbility* Ability = NewObject<UAbility>(this, AbilityClass);
			uint8 AbilityID = NextAbilityID++;
			Ability->Initialize(AbilityName, AbilityID, this);
			Ability->Activate();
			ActiveAbilitiesMap.Add(AbilityID, Ability);

			if (!ActiveAbilityNameIDsMap.Contains(AbilityName))
			{
				TArray<uint8> AbilityIDs = TArray<uint8>();
				ActiveAbilityNameIDsMap.Add(AbilityName, AbilityIDs);
			}
			ActiveAbilityNameIDsMap[AbilityName].Add(AbilityID);

			ActivatedAbilityID = AbilityID;
			return true;
		}
	}

	return false;
}

void UGameAbilitySystemComponent::FinishAbility(const FString& AbilityName = FString(""), const uint8& AbilityID = 0)
{
	if (!AbilityName.Equals(FString("")))
	{
		if (ActiveAbilityNameIDsMap.Contains(AbilityName))
		{
			TArray<uint8> IDs = ActiveAbilityNameIDsMap[AbilityName];

			if (IDs.Num() > 0)
			{
				if (IDs.Contains(AbilityID) && ActiveAbilitiesMap.Contains(AbilityID))
				{
					ActiveAbilitiesMap[AbilityID]->Finish();
					ActiveAbilitiesMap.Remove(AbilityID);
					ActiveAbilityNameIDsMap[AbilityName].Remove(AbilityID);
				}
				else if (ActiveAbilitiesMap.Contains(IDs[0]))
				{
					ActiveAbilitiesMap[IDs[0]]->Finish();
					ActiveAbilitiesMap.Remove(IDs[0]);
					ActiveAbilityNameIDsMap[AbilityName].Remove(IDs[0]);
				}
			}
		}
	}

	else if (ActiveAbilitiesMap.Contains(AbilityID))
	{
		FString Name = ActiveAbilitiesMap[AbilityID]->AbilityName;
		ActiveAbilitiesMap[AbilityID]->Finish();
		ActiveAbilitiesMap.Remove(AbilityID);

		if (ActiveAbilityNameIDsMap.Contains(Name))
		{
			if (ActiveAbilityNameIDsMap[Name].Contains(AbilityID))
			{
				ActiveAbilityNameIDsMap[Name].Remove(AbilityID);
			}
		}
	}
}

void UGameAbilitySystemComponent::PrintActiveAbilities() const
{
	FString AbilityIDsString = FString("Ability IDs: ");
	FString AbilityNameIDsString = FString("Ability Name IDs Map Contents: ");

	for (auto& Pair : ActiveAbilitiesMap)
	{
		AbilityIDsString = AbilityIDsString.Append(FString::FromInt(Pair.Key));
		AbilityIDsString = AbilityIDsString.Append(FString(" | "));
	}

	for (auto& Pair : ActiveAbilityNameIDsMap)
	{
		FString IDsString = "";
		for (uint8 AbilityID : Pair.Value)
		{
			IDsString = IDsString.Append(FString::FromInt(AbilityID));
			IDsString = IDsString.Append(FString(","));
		}

		TArray<FStringFormatArg> FormatArgs;
		FormatArgs.Add(FStringFormatArg(Pair.Key));
		FormatArgs.Add(FStringFormatArg(IDsString));

		AbilityNameIDsString = AbilityNameIDsString.Append(FString::Format(TEXT("<{0}, [{1}]>"), FormatArgs));
		AbilityNameIDsString = AbilityNameIDsString.Append(FString(" || "));
	}

	UE_LOG(LogTemp, Warning, TEXT("%s"), *AbilityIDsString)
	UE_LOG(LogTemp, Warning, TEXT("%s"), *AbilityNameIDsString)
}
