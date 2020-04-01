// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "GameMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Interfaces/MovementIntent.h"

// Sets default values for this component's properties
UGameMovementComponent::UGameMovementComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Set movement speeds.
	DefaultSpeed = MaxWalkSpeed;
	DefaultAcceleration = MaxAcceleration;
	DefaultTurnRate = RotationRate.Yaw;
	MovementIntent = FVector();
}

// Called when the game starts
void UGameMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UGameMovementComponent::SetupMovement(float TargetSpeed, float TargetAcceleration, float TargetTurnRate)
{
	SetMovementSpeed(TargetSpeed);
	SetAcceleration(TargetAcceleration);
	SetTurningRate(TargetTurnRate);
}

void UGameMovementComponent::Move(FVector BaseDirection, FVector MovementVector)
{
	FRotator AttackRotation = BaseDirection.Rotation();
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(AttackRotation) * MovementVector.X;
	FVector SideVector = UKismetMathLibrary::GetRightVector(AttackRotation) * MovementVector.Y;
	FVector UpVector = UKismetMathLibrary::GetUpVector(AttackRotation) * MovementVector.Z;

	FVector Direction = ForwardVector + SideVector + UpVector;

	AddInputVector(Direction, false);
}

void UGameMovementComponent::FinishMovement()
{
	ResetMovementSpeed();
	ResetTurningRate();
	ResetAcceleration();
}

void UGameMovementComponent::SetMovementIntent()
{
	AActor* Owner = GetOwner();
	if (Owner->GetClass()->ImplementsInterface(UMovementIntent::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Implements required class!"))
		MovementIntent = IMovementIntent::Execute_GetMovementIntent(Owner);
	}
	else
	{
		MovementIntent = FVector();
	}
}

void UGameMovementComponent::SetMovementSpeed(float Speed)
{
	MaxWalkSpeed = Speed;
}

void UGameMovementComponent::ResetMovementSpeed()
{
	MaxWalkSpeed = DefaultSpeed;
}

void UGameMovementComponent::SetAcceleration(float TargetAcceleration)
{
	MaxAcceleration = TargetAcceleration;
}

void UGameMovementComponent::ResetAcceleration()
{
	MaxAcceleration = DefaultAcceleration;
}

void UGameMovementComponent::SetTurningRate(float Rate)
{
	RotationRate = FRotator(0.0f, Rate, 0.0f);
}

void UGameMovementComponent::ResetTurningRate()
{
	RotationRate = FRotator(0.0f, DefaultTurnRate, 0.0f);
}

void UGameMovementComponent::SetGravity(float GravityValue)
{
	GravityScale = GravityValue;
}

void UGameMovementComponent::ResetGravity()
{
	GravityScale = 1.0f;
}

void UGameMovementComponent::SetFlyable()
{
	if (MovementMode != EMovementMode::MOVE_Flying)
	{
		MovementMode = EMovementMode::MOVE_Flying;
	}
}

void UGameMovementComponent::ResetFlyable()
{
	if (MovementMode == EMovementMode::MOVE_Flying)
	{
		MovementMode = EMovementMode::MOVE_Walking;
	}
}

void UGameMovementComponent::PerformMove(const FVector& InVelocity, const float DeltaSeconds)
{
	FVector MovementDirectionForward = GetOwner()->GetActorForwardVector().GetSafeNormal();
	if (!MovementIntent.IsNearlyZero(0.01f))
	{
		MovementDirectionForward = MovementIntent.GetSafeNormal();
	}
	FVector MovementDirectionRight = MovementDirectionForward.RightVector.GetSafeNormal();
	FVector MovementDirectionUp = MovementDirectionForward.UpVector.GetSafeNormal();

	FVector ForwardVelocity = MovementDirectionForward * InVelocity.X;
	FVector RightVelocity = MovementDirectionRight * InVelocity.Y;
	FVector UpVelocity = MovementDirectionUp * InVelocity.Z;

	FVector ResultantVelocity = ForwardVelocity + RightVelocity + UpVelocity;

	MoveSmooth(ResultantVelocity, DeltaSeconds);
}
