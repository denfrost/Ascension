// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "PreciseMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UPreciseMovementComponent::UPreciseMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// Set movement speeds.
	BaseSpeed = 500.0f;
	BaseAcceleration = 2048.0f;
	BaseTurnRate = 540.0f;
}

void UPreciseMovementComponent::Initialize(const float DefaultSpeed, const float DefaultAcceleration, const float DefaultTurnRate)
{
	BaseSpeed = DefaultSpeed;
	BaseAcceleration = DefaultAcceleration;
	BaseTurnRate = DefaultTurnRate;
}

// Called when the game starts
void UPreciseMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ACharacter>(GetOwner());
}

void UPreciseMovementComponent::SetupMovement(float Speed, float Acceleration, float TurnRate)
{
	SetMovementSpeed(Speed);
	SetAcceleration(Acceleration);
	SetTurningRate(TurnRate);
}

void UPreciseMovementComponent::Move(FVector BaseDirection, FVector MovementVector)
{
	FRotator AttackRotation = BaseDirection.Rotation();
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(AttackRotation) * MovementVector.X;
	FVector SideVector = UKismetMathLibrary::GetRightVector(AttackRotation) * MovementVector.Y;
	FVector UpVector = UKismetMathLibrary::GetUpVector(AttackRotation) * MovementVector.Z;

	FVector Direction = ForwardVector + SideVector + UpVector;

	Owner->AddMovementInput(Direction);
}

void UPreciseMovementComponent::FinishMovement()
{
	ResetMovementSpeed();
	ResetTurningRate();
	ResetAcceleration();
}

void UPreciseMovementComponent::SetMovementSpeed(float Speed)
{
	Owner->GetCharacterMovement()->MaxWalkSpeed = Speed;
}

void UPreciseMovementComponent::ResetMovementSpeed()
{
	Owner->GetCharacterMovement()->MaxWalkSpeed = BaseSpeed;
}

void UPreciseMovementComponent::SetAcceleration(float Acceleration)
{
	Owner->GetCharacterMovement()->MaxAcceleration = Acceleration;
}

void UPreciseMovementComponent::ResetAcceleration()
{
	Owner->GetCharacterMovement()->MaxAcceleration = BaseAcceleration;
}

void UPreciseMovementComponent::SetTurningRate(float Rate)
{
	Owner->GetCharacterMovement()->RotationRate = FRotator(0.0f, Rate, 0.0f);
}

void UPreciseMovementComponent::ResetTurningRate()
{
	Owner->GetCharacterMovement()->RotationRate = FRotator(0.0f, BaseTurnRate, 0.0f);
}

void UPreciseMovementComponent::SetGravity(float GravityValue)
{
	Owner->GetCharacterMovement()->GravityScale = GravityValue;
}

void UPreciseMovementComponent::ResetGravity()
{
	Owner->GetCharacterMovement()->GravityScale = 1.0f;
}

void UPreciseMovementComponent::SetFlyable()
{
	if (Owner->GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Flying)
	{
		Owner->GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Flying;
	}
}

void UPreciseMovementComponent::ResetFlyable()
{
	if (Owner->GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Flying)
	{
		Owner->GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Walking;
	}
}
