// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "Damageable.h"


// This function does not need to be modified.
UDamageable::UDamageable(const class FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

// Add default functionality here for any IDamageable functions that are not pure virtual.
void IDamageable::GetHealthPercent_Implementation(float& HealthPercent) {}

void IDamageable::ApplyHitEffect_Implementation(const AActor* SourceActor, const float Damage, const EHitEffect HitEffect, const FAttackEffect AttackEffect) {}

void IDamageable::ShowHealthBar_Implementation() {}

void IDamageable::HideHealthBar_Implementation() {}

bool IDamageable::IsDead_Implementation() 
{
	return false;
}