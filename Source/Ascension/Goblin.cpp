// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "Goblin.h"


AGoblin::AGoblin()
{
	// Set character parameters.
	Health = 100.0f;
	MaxHealth = 100.0f;
}

void AGoblin::GetHealthPercent_Implementation(float& HealthPercent)
{
	HealthPercent = Health / MaxHealth;
}

void AGoblin::ApplyHitEffect_Implementation(const float Damage, const EHitEffect HitEffect)
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

void AGoblin::ShowHitVisuals_Implementation() {}
void AGoblin::ApplyAttackEffects_Implementation(float Damage) {}

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


