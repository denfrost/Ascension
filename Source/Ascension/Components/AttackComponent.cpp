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

	// Set movement speeds.
	ActionTurnRate = 2048.0f;

	// Set gameplay variables.
	AttackHitBox = nullptr;
	DamagedActors.Empty();

	// Set currently active attack.
	ActiveAttack = nullptr;

	// Clear maps and arrays.
	/*
	Attacks.Empty();
	AttackNameMap.Empty();
	*/
}


void UAttackComponent::Initialize(const float ActionTurnRate)
{
	this->ActionTurnRate = ActionTurnRate;
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

bool UAttackComponent::CanAttack()
{
	return false;
}

/*
void UAttackComponent::CreateAttack_Implementation(const FString& AttackName, const FAttackStruct& Attack)
{
	Attacks.Add(Attack);
	int AttackIndex = Attacks.Num() - 1;

	AttackNameMap.Add(AttackName, AttackIndex);
}

void UAttackComponent::GetAttack(const FString& AttackName, bool& Found, FAttackStruct& Attack)
{
	int* Index = AttackNameMap.Find(AttackName);
	if (Index)
	{
		Attack = Attacks[*Index];
		Found = true;
	}
	else
	{
		Attack = NullAttack;
		Found = false;
	}
}

void UAttackComponent::SetAttack(const bool& Found, const FAttackStruct& Attack)
{
	if (Found)
	{
		AttackToPerform = Attack;
	}
	else
	{
		AttackToPerform = NullAttack;
	}
}
*/

void UAttackComponent::SetupMotion()
{
	if (ActiveAttack != nullptr)
	{
		UGameAbilitySystemComponent* AbilitySystem = Owner->FindComponentByClass<UGameAbilitySystemComponent>();

		if (AbilitySystem)
		{
			UAttack* Attack = Cast<UAttack>(AbilitySystem->GetAbility(*ActiveAttack));
			UGameMovementComponent* MovementComponent = Owner->FindComponentByClass<UGameMovementComponent>();

			if (Attack != nullptr && MovementComponent != nullptr)
			{
				MovementComponent->SetupMovement(Attack->GetMovementInfo().Speed, 
												 Attack->GetMovementInfo().Acceleration, 
												 ActionTurnRate);
			}
		}

		/*
		UGameMovementComponent* MovementComponent = Owner->FindComponentByClass<UGameMovementComponent>();
		if (MovementComponent != nullptr)
		{
			MovementComponent->SetupMovement(AttackToPerform.Speed, AttackToPerform.Acceleration, ActionTurnRate);
		}
		*/
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
	UAttack* Attack = SelectAttack(AttackName);

	if (Attack != nullptr)
	{
		if (ActiveAttack != nullptr)
		{
			AbilitySystem->FinishAbility(*ActiveAttack);
		}

		ActionDirection = MovementIntent;
		AbilitySystem->ActivateAbility(Attack->AbilityName);
	}

	/*
	AttackToPerform = NullAttack;
	SelectAttack(AttackName);

	if (AttackToPerform.AnimMontage != nullptr)
	{
		ActionDirection = MovementIntent;
		Owner->PlayAnimMontage(AttackToPerform.AnimMontage);
	}
	*/
}

UAttack* UAttackComponent::SelectAttack_Implementation(const FString& AttackType)
{
	UGameAbilitySystemComponent* AbilitySystem = Owner->FindComponentByClass<UGameAbilitySystemComponent>();
	UAttack* Attack = nullptr;

	if (AbilitySystem)
	{
		Attack = Cast<UAttack>(AbilitySystem->GetAbility(AttackType));
	}

	return Attack;
}

void UAttackComponent::Reset_Implementation()
{
	ResetFlyable();
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

						if (ActiveAttack != nullptr && AbilitySystem != nullptr)
						{
							UAttack* Attack = Cast<UAttack>(AbilitySystem->GetAbility(*ActiveAttack));

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

void UAttackComponent::SetFlyable_Implementation()
{
	if (Owner->GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Flying)
	{
		Owner->GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Flying;
	}
}

void UAttackComponent::ResetFlyable_Implementation()
{
	if (Owner->GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Flying)
	{
		Owner->GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Walking;
	}
}

void UAttackComponent::FinalizeAttackDirection_Implementation(FVector MovementIntent)
{
	ActionDirection = MovementIntent;
}
