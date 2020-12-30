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
	ControlledMovementInstanceID = 0;
	AbilityNameIDsMap.Empty();
}

// Called when the game starts
void UGameMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

int UGameMovementComponent::SetupControlledMovement(float TargetSpeed, float TargetAcceleration, float TargetTurnRate,
													float MaxTurnAngleDegrees = 0.0f, bool HasZMovement = false)
{
	SetMovementSpeed(TargetSpeed);
	SetAcceleration(TargetAcceleration);
	SetTurningRate(TargetTurnRate);

	if (HasZMovement)
	{
		SetFlyable();
	}

	AActor* Owner = GetOwner();
	if (Owner->Implements<UGameMovementInterface>())
	{
		FVector ActorForwardVector = Owner->GetActorForwardVector();
		FVector DesiredMovementDirection = IGameMovementInterface::Execute_GetMovementDirection(Owner);

		ActorForwardVector.Normalize();
		DesiredMovementDirection.Normalize();

		float Angle = UKismetMathLibrary::DegAcos(FVector::DotProduct(ActorForwardVector, DesiredMovementDirection));
		FVector CrossProductVector = FVector::CrossProduct(ActorForwardVector, DesiredMovementDirection);

		Angle = Angle < MaxTurnAngleDegrees ? Angle : MaxTurnAngleDegrees;
		Angle = CrossProductVector.Z > 0 ? Angle : -Angle;

		FVector DirectionToMove = ActorForwardVector.RotateAngleAxis(Angle, FVector(0.0f, 0.0f, 1.0f));
		DirectionToMove.Normalize();

		SetMovementDirection(DirectionToMove);
	}
	else
	{
		SetMovementDirection(Owner->GetActorForwardVector());
	}

	return ++ControlledMovementInstanceID;
}

int UGameMovementComponent::SetupControlledMovementAbility(FString AbilityName)
{
	AActor* Owner = GetOwner();
	UGameAbilitySystemComponent* AbilitySystemComponent = Owner->FindComponentByClass<UGameAbilitySystemComponent>();

	const UAbility* Ability = AbilitySystemComponent->GetActiveAbility(AbilityName, 0);
	if (Ability != nullptr)
	{
		FCustomMovementParams MovementParams = Ability->GetMovementParams();
		int MovementID = SetupControlledMovement(MovementParams.Speed, MovementParams.Acceleration, MovementParams.TurnRate,
												 MovementParams.MaxTurnAngleDegrees, MovementParams.HasZMovement);

		if (!AbilityNameIDsMap.Contains(AbilityName))
		{
			TArray<int> IDs = TArray<int>();
			AbilityNameIDsMap.Add(AbilityName, IDs);
		}

		AbilityNameIDsMap[AbilityName].Add(MovementID);

		return MovementID;
	}

	return 0;
}

void UGameMovementComponent::ControlledMove(FVector MovementVector)
{
	// Movement direction is the direction in relation to which the controlled movement is applied.
	FRotator MovementRotation = MovementDirection.Rotation();
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(MovementRotation) * MovementVector.X;
	FVector SideVector = UKismetMathLibrary::GetRightVector(MovementRotation) * MovementVector.Y;
	FVector UpVector = UKismetMathLibrary::GetUpVector(MovementRotation) * MovementVector.Z;

	FVector Direction = ForwardVector + SideVector + UpVector;
	AddInputVector(Direction, false);
}

void UGameMovementComponent::FinishControlledMovement(int InstanceID = 0)
{
	if (ControlledMovementInstanceID == InstanceID)
	{
		ResetMovementSpeed();
		ResetTurningRate();
		ResetAcceleration();
		ResetFlyable();
		ControlledMovementInstanceID = 0;
	}
}

void UGameMovementComponent::FinishControlledMovementAbility(FString AbilityName = FString(""), int InstanceID = 0)
{
	if (!AbilityName.Equals(FString("")))
	{
		if (AbilityNameIDsMap.Contains(AbilityName))
		{
			TArray<int> IDs = AbilityNameIDsMap[AbilityName];

			if (IDs.Num() > 0)
			{
				if (IDs.Contains(InstanceID))
				{
					FinishControlledMovement(InstanceID);
					AbilityNameIDsMap[AbilityName].Remove(InstanceID);
				}
				else
				{
					FinishControlledMovement(IDs[0]);
					AbilityNameIDsMap[AbilityName].Remove(IDs[0]);
				}
			}
		}
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
		MovementMode = EMovementMode::MOVE_Falling;
	}
}
