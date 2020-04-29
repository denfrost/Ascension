// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "PlayerStateComponent.h"


// Sets default values for this component's properties
UPlayerStateComponent::UPlayerStateComponent()
{
	// Set character states.
	CharacterState = ECharacterState::CS_Idle;
	MovementState = EMovementState::MS_OnGround;
	WeaponState = EWeaponState::WS_Sheathed;
}

// Called when the game starts
void UPlayerStateComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
}

ECharacterState UPlayerStateComponent::GetCharacterState() const
{
	return CharacterState;
}

EMovementState UPlayerStateComponent::GetMovementState() const
{
	return MovementState;
}

EWeaponState UPlayerStateComponent::GetWeaponState() const
{
	return WeaponState;
}

void UPlayerStateComponent::SetCharacterState(ECharacterState State)
{
	CharacterState = State;
}

void UPlayerStateComponent::SetMovementState(EMovementState State)
{
	MovementState = State;
}

void UPlayerStateComponent::SetWeaponState(EWeaponState State)
{
	WeaponState = State;
}
