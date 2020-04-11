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
	ActiveAttackIDMap.Empty();
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

void UAttackComponent::SetupMotion()
{
	if (!ActiveAttack.IsEmpty())
	{
		UGameAbilitySystemComponent* AbilitySystem = Owner->FindComponentByClass<UGameAbilitySystemComponent>();

		if (AbilitySystem)
		{
			UAttack* Attack = Cast<UAttack>(AbilitySystem->GetAbility(ActiveAttack));
			UGameMovementComponent* MovementComponent = Owner->FindComponentByClass<UGameMovementComponent>();

			if (Attack != nullptr && MovementComponent != nullptr)
			{
				MovementComponent->SetupMovement(Attack->GetMovementInfo().Speed, 
												 Attack->GetMovementInfo().Acceleration, 
												 Attack->GetMovementInfo().TurnRate);
			}
		}
	}
}

void UAttackComponent::AttackMotion(FVector MovementVector)
{
	UGameMovementComponent* MovementComponent = Owner->FindComponentByClass<UGameMovementComponent>();
	if (MovementComponent != nullptr)
	{
		MovementComponent->Move(ActionDirection, MovementVector);
	}
}

void UAttackComponent::FinishMotion()
{
	UGameMovementComponent* MovementComponent = Owner->FindComponentByClass<UGameMovementComponent>();
	if (MovementComponent != nullptr)
	{
		MovementComponent->FinishMovement();
	}
}

void UAttackComponent::Attack_Implementation(const FString& AttackName, const FVector& MovementIntent)
{
	UGameAbilitySystemComponent* AbilitySystem = Owner->FindComponentByClass<UGameAbilitySystemComponent>();

	if (AbilitySystem->CanActivateAbility(AttackName))
	{
		if (!ActiveAttack.IsEmpty())
		{
			AbilitySystem->FinishAbility(ActiveAttack);
		}

		ActionDirection = MovementIntent;
		AbilitySystem->ActivateAbility(AttackName);
		ActiveAttack = Attack->AbilityName;
	}
}

void UAttackComponent::FinishAttack_Implementation(const FString& AttackName)
{
	ActiveAttack = FString();
	AbilitySystem->FinishAbility(AttackName);
}

void UAttackComponent::DetectHit()
{
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

						if (!ActiveAttack.IsEmpty() && AbilitySystem != nullptr)
						{
							UAttack* Attack = Cast<UAttack>(AbilitySystem->GetAbility(ActiveAttack));

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
}

void UAttackComponent::ClearDamagedActors_Implementation()
{
	DamagedActors.Empty();
}

void UAttackComponent::FinalizeAttackDirection_Implementation(FVector MovementIntent)
{
	ActionDirection = MovementIntent;
}
