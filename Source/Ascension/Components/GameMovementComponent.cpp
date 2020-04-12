// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "GameMovementComponent.h"
#include "Interfaces/GameMovementInterface.h"
#include "Abilities/Attacks/Attack.h"
#include "Abilities/AbilitySystems/GameAbilitySystemComponent.h"
#include "Kismet/KismetMathLibrary.h"

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
	MovementDirection = FVector();
	MovementStateLock = FString();
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

	AActor* Owner = GetOwner();
	if (Owner->Implements<UGameMovementInterface>())
	{
		SetMovementDirection(IGameMovementInterface::Execute_GetMovementDirection(Owner));
	}
	else
	{
		SetMovementDirection(Owner->GetActorForwardVector());
	}
}

void UGameMovementComponent::SetupMovementAbility(FString AbilityName)
{
	AActor* Owner = GetOwner();
	UGameAbilitySystemComponent* AbilitySystemComponent = Owner->FindComponentByClass<UGameAbilitySystemComponent>();

	// TODO: Ideally, this should be done for any ability.
	const UAttack* Attack = Cast<UAttack>(AbilitySystemComponent->GetActiveAbility(AbilityName));
	if (Attack != nullptr)
	{
		FCustomMovementParams MovementParams = Attack->GetMovementParams();
		SetupMovement(MovementParams.Speed, MovementParams.Acceleration, MovementParams.TurnRate);
		MovementStateLock = AbilityName;
	}
}

void UGameMovementComponent::Move(FVector MovementVector)
{
	// Movement direction is the direction with respect to which the movement vector is applied.
	FRotator MovementRotation = MovementDirection.Rotation();
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(MovementRotation) * MovementVector.X;
	FVector SideVector = UKismetMathLibrary::GetRightVector(MovementRotation) * MovementVector.Y;
	FVector UpVector = UKismetMathLibrary::GetUpVector(MovementRotation) * MovementVector.Z;

	FVector Direction = ForwardVector + SideVector + UpVector;

	AddInputVector(Direction, false);
}

void UGameMovementComponent::FinishMovement(FString AbilityName = FString())
{
	if (MovementStateLock.Equals(AbilityName))
	{
		ResetMovementSpeed();
		ResetTurningRate();
		ResetAcceleration();
		MovementStateLock = FString();
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

void UGameMovementComponent::SetMovementDirection(FVector Direction)
{
	MovementDirection = Direction;
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
	if (!MovementDirection.IsNearlyZero(0.01f))
	{
		MovementDirectionForward = MovementDirection.GetSafeNormal();
	}
	FVector MovementDirectionRight = MovementDirectionForward.RightVector.GetSafeNormal();
	FVector MovementDirectionUp = MovementDirectionForward.UpVector.GetSafeNormal();

	FVector ForwardVelocity = MovementDirectionForward * InVelocity.X;
	FVector RightVelocity = MovementDirectionRight * InVelocity.Y;
	FVector UpVelocity = MovementDirectionUp * InVelocity.Z;

	FVector ResultantVelocity = ForwardVelocity + RightVelocity + UpVelocity;

	MoveSmooth(ResultantVelocity, DeltaSeconds);
}
