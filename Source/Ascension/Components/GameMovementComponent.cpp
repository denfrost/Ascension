// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "GameMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UGameMovementComponent::UGameMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// Set movement speeds.
	BaseSpeed = 500.0f;
	BaseAcceleration = 2048.0f;
	BaseTurnRate = 540.0f;
}

void UGameMovementComponent::Initialize(const float DefaultSpeed, const float DefaultAcceleration, const float DefaultTurnRate)
{
	BaseSpeed = DefaultSpeed;
	BaseAcceleration = DefaultAcceleration;
	BaseTurnRate = DefaultTurnRate;
}

// Called when the game starts
void UGameMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ACharacter>(GetOwner());
}

void UGameMovementComponent::SetupMovement(float Speed, float Acceleration, float TurnRate)
{
	SetMovementSpeed(Speed);
	SetAcceleration(Acceleration);
	SetTurningRate(TurnRate);
}

void UGameMovementComponent::Move(FVector BaseDirection, FVector MovementVector)
{
	FRotator AttackRotation = BaseDirection.Rotation();
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(AttackRotation) * MovementVector.X;
	FVector SideVector = UKismetMathLibrary::GetRightVector(AttackRotation) * MovementVector.Y;
	FVector UpVector = UKismetMathLibrary::GetUpVector(AttackRotation) * MovementVector.Z;

	FVector Direction = ForwardVector + SideVector + UpVector;

	Owner->AddMovementInput(Direction);
}

void UGameMovementComponent::FinishMovement()
{
	ResetMovementSpeed();
	ResetTurningRate();
	ResetAcceleration();
}

void UGameMovementComponent::SetMovementSpeed(float Speed)
{
	Owner->GetCharacterMovement()->MaxWalkSpeed = Speed;
}

void UGameMovementComponent::ResetMovementSpeed()
{
	Owner->GetCharacterMovement()->MaxWalkSpeed = BaseSpeed;
}

void UGameMovementComponent::SetAcceleration(float Acceleration)
{
	Owner->GetCharacterMovement()->MaxAcceleration = Acceleration;
}

void UGameMovementComponent::ResetAcceleration()
{
	Owner->GetCharacterMovement()->MaxAcceleration = BaseAcceleration;
}

void UGameMovementComponent::SetTurningRate(float Rate)
{
	Owner->GetCharacterMovement()->RotationRate = FRotator(0.0f, Rate, 0.0f);
}

void UGameMovementComponent::ResetTurningRate()
{
	Owner->GetCharacterMovement()->RotationRate = FRotator(0.0f, BaseTurnRate, 0.0f);
}

void UGameMovementComponent::SetGravity(float GravityValue)
{
	Owner->GetCharacterMovement()->GravityScale = GravityValue;
}

void UGameMovementComponent::ResetGravity()
{
	Owner->GetCharacterMovement()->GravityScale = 1.0f;
}

void UGameMovementComponent::SetFlyable()
{
	if (Owner->GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Flying)
	{
		Owner->GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Flying;
	}
}

void UGameMovementComponent::ResetFlyable()
{
	if (Owner->GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Flying)
	{
		Owner->GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Walking;
	}
}
