// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "PlayerAttackComponent.h"


UPlayerAttackComponent::UPlayerAttackComponent() {}

void UPlayerAttackComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerAttackComponent::SelectAttack_Implementation(const FString& AttackType) {}

void UPlayerAttackComponent::Reset_Implementation()
{
	Super::Reset_Implementation();
	ComboMeter = 0;
}