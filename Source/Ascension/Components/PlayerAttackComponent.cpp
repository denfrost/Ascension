// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "PlayerAttackComponent.h"


UPlayerAttackComponent::UPlayerAttackComponent() {}

void UPlayerAttackComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerAttackComponent::SelectAttack_Implementation(const FString& AttackType)
{
	bool Found = false;
	FAttackStruct Attack;

	if (AttackType.Equals(FString("Light Attack")))
	{
		switch (ComboMeter)
		{
		case 0:
			GetAttack(FString("Light01"), Found, Attack);
			ComboMeter++;
			break;

		case 1:
			GetAttack(FString("Light02"), Found, Attack);
			ComboMeter++;
			break;

		case 2:
			GetAttack(FString("Light03"), Found, Attack);
			ComboMeter++;
			break;

		default:
			GetAttack(FString("Light01"), Found, Attack);
			ComboMeter = 1;
			break;
		}
	}

	else if (AttackType.Equals(FString("Strong Attack")))
	{
		switch (ComboMeter)
		{
		case 0:
			GetAttack(FString("Strong01"), Found, Attack);
			ComboMeter++;
			break;

		case 1:
			GetAttack(FString("Strong02"), Found, Attack);
			ComboMeter++;
			break;

		case 2:
			GetAttack(FString("Strong03"), Found, Attack);
			ComboMeter++;
			break;

		default:
			GetAttack(FString("Strong01"), Found, Attack);
			ComboMeter = 1;
			break;
		}
	}

	SetAttack(Found, Attack);
}

void UPlayerAttackComponent::Reset_Implementation()
{
	Super::Reset_Implementation();
	ComboMeter = 0;
}