// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "Goblin.h"
#include "Components/AttackComponent.h"


AGoblin::AGoblin()
{
	// Set character parameters.
	Health = 100.0f;
	MaxHealth = 100.0f;

	// Initialize components.
	AttackComponent = nullptr;
}

void AGoblin::GetHealthPercent_Implementation(float& HealthPercent)
{
	HealthPercent = Health / MaxHealth;
}

void AGoblin::ApplyHitEffect_Implementation(const AActor* SourceActor, const float Damage, const EHitEffect HitEffect, const FAttackEffect AttackEffect)
{
}

void AGoblin::ShowHealthBar_Implementation()
{

}

void AGoblin::HideHealthBar_Implementation()
{

}

bool AGoblin::IsDead_Implementation()
{
	return Dead;
}

void AGoblin::FootstepSound_Implementation() {}

void AGoblin::EnableMovement_Implementation()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AGoblin::DisableMovement_Implementation()
{
	GetCharacterMovement()->StopMovementImmediately();
}

bool AGoblin::CheckDead()
{
	Health <= 0 ? Dead = true : Dead = false;
	return Dead;
}

void AGoblin::EnterCombat(AActor* Enemy)
{
	UBlackboardComponent* Blackboard = UAIBlueprintHelperLibrary::GetBlackboard(this);
	
	Blackboard->SetValueAsObject(EnemyKeyName, Enemy);

	AIState = EAIState::AIS_Combat;
	Blackboard->SetValueAsEnum(AIStateKeyName, (uint8) AIState);

	CombatState = EEnemyCombatState::ECS_Observing;
	Blackboard->SetValueAsEnum(CombatStateKeyName, (uint8) CombatState);
}

void AGoblin::ExitCombat()
{
	UBlackboardComponent* Blackboard = UAIBlueprintHelperLibrary::GetBlackboard(this);

	Blackboard->ClearValue(EnemyKeyName);

	AIState = EAIState::AIS_Patrol;
	Blackboard->SetValueAsEnum(AIStateKeyName, (uint8) AIState);
}

void AGoblin::ShowHitVisuals_Implementation() {}

void AGoblin::Attack_Implementation()
{
	if (AttackComponent)
	{
		if (ActionState == EEnemyState::ES_Idle)
		{
			ActionState = EEnemyState::ES_Attacking;
			GetCharacterMovement()->StopMovementImmediately();

			AttackComponent->Attack(FString(), GetActorForwardVector());
		}
	}
}

void AGoblin::ResetAttack_Implementation()
{
	ActionState = EEnemyState::ES_Idle;

	if (AttackComponent)
	{
		AttackComponent->Reset();
	}
}

void AGoblin::AttackComplete_Implementation()
{

}

void AGoblin::ApplyAttackEffects_Implementation(const AActor* SourceActor, float Damage, const EHitEffect HitEffect, const FAttackEffect AttackEffect)
{
	DecrementHealth(Damage);

	if (SourceActor != nullptr)
	{
		// Find the direction in which we need to launch our character.
		FVector SourceLocation = SourceActor->GetActorLocation();
		FVector HitLocation = GetActorLocation();
		FVector LaunchDirection = HitLocation - SourceLocation;
		LaunchDirection.Normalize();

		// Calculate launch velocity.
		FVector LaunchVelocity = LaunchDirection * AttackEffect.KnockbackForce;

		switch (HitEffect)
		{
		// Launch character backwards.
		case EHitEffect::HE_PushBack:
			LaunchCharacter(LaunchVelocity, true, false);
			break;

		case EHitEffect::HE_KnockBack:
			break;

		case EHitEffect::HE_LaunchUp:
			break;

		default:
			break;
		}
	}
}

void AGoblin::KillActor_Implementation()
{
	Dead = true;
	DetachFromControllerPendingDestroy();
	DisableMovement();
}

void AGoblin::DecrementHealth_Implementation(float Damage)
{
	Health -= Damage;
}


