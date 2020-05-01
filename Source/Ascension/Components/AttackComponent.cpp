// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "AttackComponent.h"
#include "GameMovementComponent.h"
#include "Interfaces/Damageable.h"
#include "Abilities/AbilitySystems/GameAbilitySystemComponent.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values for this component's properties
UAttackComponent::UAttackComponent()
{
	// Set this component to be initialized when the game starts, and to not be ticked every frame.
	PrimaryComponentTick.bCanEverTick = true;

	// Set gameplay variables.
	AttackHitBox = nullptr;
	DamagedActors.Empty();

	// Clear active attacks.
	ActiveAttackIDs.Empty();
	ActiveAttackNameIDsMap.Empty();
}


void UAttackComponent::Initialize()
{
}

// Called when the actor is in play.
void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ACharacter>(GetOwner());
}

void UAttackComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UAttackComponent::SetupAttack_Implementation(const FString& AttackName = FString(""), const uint8 AttackID = 0) {}

bool UAttackComponent::Attack_Implementation(const FString& AttackName)
{
	UGameAbilitySystemComponent* AbilitySystem = Owner->FindComponentByClass<UGameAbilitySystemComponent>();

	if (AbilitySystem)
	{
		if (AbilitySystem->CanActivateAbility(AttackName))
		{
			uint8 AttackID = 0;
			bool Activated = AbilitySystem->ActivateAbility(AttackName, AttackID);

			if (Activated)
			{
				ActiveAttackIDs.Add(AttackID);

				if (!ActiveAttackNameIDsMap.Contains(AttackName))
				{
					TArray<uint8> AttackIDs = TArray<uint8>();
					ActiveAttackNameIDsMap.Add(AttackName, AttackIDs);
				}
				ActiveAttackNameIDsMap[AttackName].Add(AttackID);

				return true;
			}
		}
	}

	return false;
}

void UAttackComponent::FinishAttack_Implementation(const FString& AttackName = FString(""), const uint8 AttackID = 0)
{
	UGameAbilitySystemComponent* AbilitySystem = Owner->FindComponentByClass<UGameAbilitySystemComponent>();

	if (!AttackName.Equals(FString("")))
	{
		if (ActiveAttackNameIDsMap.Contains(AttackName))
		{
			TArray<uint8> IDs = ActiveAttackNameIDsMap[AttackName];

			if (IDs.Num() > 0)
			{
				if (IDs.Contains(AttackID) && ActiveAttackIDs.Contains(AttackID))
				{
					if (AbilitySystem)
					{
						AbilitySystem->FinishAbility(AttackName, AttackID);
						ActiveAttackIDs.Remove(AttackID);
						ActiveAttackNameIDsMap[AttackName].Remove(AttackID);
					}
				}
				else if (ActiveAttackIDs.Contains(IDs[0]))
				{
					if (AbilitySystem)
					{
						AbilitySystem->FinishAbility(AttackName, IDs[0]);
						ActiveAttackIDs.Remove(IDs[0]);
						ActiveAttackNameIDsMap[AttackName].Remove(IDs[0]);
					}
				}
			}
		}
	}

	else if (ActiveAttackIDs.Contains(AttackID))
	{
		if (AbilitySystem)
		{
			AbilitySystem->FinishAbility(AttackName, AttackID);
			ActiveAttackIDs.Remove(AttackID);

			if (ActiveAttackNameIDsMap.Contains(AttackName))
			{
				if (ActiveAttackNameIDsMap[AttackName].Contains(AttackID))
				{
					ActiveAttackNameIDsMap[AttackName].Remove(AttackID);
				}
			}
		}
	}
}

void UAttackComponent::DetectHit()
{
	// TODO: Rework this.
	/*
	if (AttackHitBox)
	{
		TArray<UPrimitiveComponent*> OverlappingComponents;
		AttackHitBox->GetOverlappingComponents(OverlappingComponents);

		for (int i = 0; i < OverlappingComponents.Num(); i++)
		{
			UPrimitiveComponent* OtherComponent = OverlappingComponents[i];
			if (OtherComponent->ComponentHasTag(FName("Hitbox")))
			{
				AActor* OtherActor = OtherComponent->GetOwner();

				if (DamagedActors.Find(OtherActor) == INDEX_NONE && OtherActor != GetOwner())
				{
					if (OtherActor->GetClass()->ImplementsInterface(UDamageable::StaticClass()))
					{
						UGameAbilitySystemComponent* AbilitySystem = Owner->FindComponentByClass<UGameAbilitySystemComponent>();

						if (!(ActiveAttacks.Num() == 0) && AbilitySystem != nullptr)
						{
							// Get the correct attack, not the zeroth index.
							UAttack* Attack = Cast<UAttack>(AbilitySystem->GetAbility(ActiveAttacks[0]));

							if (Attack != nullptr)
							{
								IDamageable::Execute_ApplyHitEffect(OtherActor, GetOwner(), 
																	Attack->GetEffectInfo().Damage, 
																	Attack->GetEffectInfo().HitEffect, 
																	Attack->GetEffectInfo().AttackEffect);
								DamagedActors.Add(OtherActor);
							}
						}
					}
				}
			}
		}
	}
	*/
}

void UAttackComponent::ClearDamagedActors_Implementation()
{
	DamagedActors.Empty();
}

void UAttackComponent::FinalizeAttackDirection_Implementation(FVector MovementIntent)
{
	ActionDirection = MovementIntent;
}

void UAttackComponent::PrintActiveAttacks() const
{
	FString AttackIDsString = FString("Attack IDs: ");
	FString AttackNameIDsString = FString("Attack Name IDs Map Contents: ");

	for (uint8 AttackID : ActiveAttackIDs)
	{
		AttackIDsString = AttackIDsString.Append(FString::FromInt(AttackID));
		AttackIDsString = AttackIDsString.Append(FString(" | "));
	}

	for (auto& Pair : ActiveAttackNameIDsMap)
	{
		FString IDsString = "";
		for (uint8 AttackID : Pair.Value)
		{
			IDsString = IDsString.Append(FString::FromInt(AttackID));
			IDsString = IDsString.Append(FString(","));
		}

		TArray<FStringFormatArg> FormatArgs;
		FormatArgs.Add(FStringFormatArg(Pair.Key));
		FormatArgs.Add(FStringFormatArg(IDsString));

		AttackNameIDsString = AttackNameIDsString.Append(FString::Format(TEXT("<{0}, [{1}]>"), FormatArgs));
		AttackNameIDsString = AttackNameIDsString.Append(FString(" || "));
	}

	UE_LOG(LogTemp, Warning, TEXT("%s"), *AttackIDsString)
	UE_LOG(LogTemp, Warning, TEXT("%s"), *AttackNameIDsString)
}
