// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "AttackComponent.h"
#include "Interfaces/Damageable.h"


// Sets default values for this component's properties
UAttackComponent::UAttackComponent()
{
	// Set this component to be initialized when the game starts, and to not be ticked every frame.
	PrimaryComponentTick.bCanEverTick = false;

	// Set movement speeds.
	NormalSpeed = 500.0f;
	NormalAcceleration = 2048.0f;
	NormalTurnRate = 540.0f;
	ActionTurnRate = 2048.0f;

	// Setup the movement timelines.
	TimelineToPlay = nullptr;

	// Clear maps and arrays.
	Attacks.Empty();
	AttackTimelines.Empty();
	AttackNameMap.Empty();
}


// Called when the game starts
void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ACharacter>(GetOwner());
}

void UAttackComponent::CreateAttack_Implementation(const FString& AttackName, const FAttack& Attack)
{
	Attacks.Add(Attack);
	int AttackIndex = Attacks.Num() - 1;

	UTimelineComponent* AttackTimeline = NewObject<UTimelineComponent>(this, FName(*AttackName));
	AttackTimeline->RegisterComponent();
	SetupTimelineComponent(AttackTimeline, Attack.MovementCurve);
	AttackTimelines.Add(AttackTimeline);

	AttackNameMap.Add(AttackName, AttackIndex);
}

void UAttackComponent::GetAttack(const FString& AttackName, bool& Found, FAttack& Attack, UTimelineComponent*& AttackTimeline)
{
	int* Index = AttackNameMap.Find(AttackName);
	if (Index)
	{
		Attack = Attacks[*Index];
		AttackTimeline = AttackTimelines[*Index];
		Found = true;
	}
	else
	{
		Attack = NullAttack;
		AttackTimeline = nullptr;
		Found = false;
	}
}

void UAttackComponent::SetAttack(const bool& Found, const FAttack& Attack, UTimelineComponent* AttackTimeline)
{
	if (Found)
	{
		AttackToPerform = Attack;

		if (TimelineToPlay != nullptr)
		{
			TimelineToPlay->Stop();
		}

		TimelineToPlay = AttackTimeline;
	}
	else
	{
		AttackToPerform = NullAttack;
	}
}

void UAttackComponent::StopMovement()
{
	Owner->GetCharacterMovement()->MaxWalkSpeed = 0.0f;
}

void UAttackComponent::SetMovementSpeed(float Speed)
{
	Owner->GetCharacterMovement()->MaxWalkSpeed = Speed;
}

void UAttackComponent::ResetMovementSpeed()
{
	Owner->GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
}

void UAttackComponent::SetAcceleration(float Acceleration)
{
	Owner->GetCharacterMovement()->MaxAcceleration = Acceleration;
}

void UAttackComponent::ResetAcceleration()
{
	Owner->GetCharacterMovement()->MaxAcceleration = NormalAcceleration;
}

void UAttackComponent::StopTurning()
{
	Owner->GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 0.0f);
}

void UAttackComponent::SetTurningRate(float Rate)
{
	Owner->GetCharacterMovement()->RotationRate = FRotator(0.0f, Rate, 0.0f);
}

void UAttackComponent::ResetTurningRate()
{
	Owner->GetCharacterMovement()->RotationRate = FRotator(0.0f, NormalTurnRate, 0.0f);
}

void UAttackComponent::SetGravity(float GravityValue)
{
	Owner->GetCharacterMovement()->GravityScale = GravityValue;
}

void UAttackComponent::ResetGravity()
{
	Owner->GetCharacterMovement()->GravityScale = 1.0f;
}

void UAttackComponent::TimelineMovement(float Speed)
{
	Owner->AddMovementInput(ActionDirection, Speed);
}

void UAttackComponent::SetupTimelineComponent(UTimelineComponent* TimelineComponent, UCurveFloat* MovementCurve)
{
	if (MovementCurve != nullptr)
	{
		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindUFunction(this, FName("TimelineMovement"));

		TimelineComponent->AddInterpFloat(MovementCurve, ProgressFunction);
	}
}


void UAttackComponent::Attack_Implementation(const FString& AttackName, const FVector& MovementIntent)
{
	AttackToPerform = NullAttack;
	SelectAttack(AttackName);

	if (AttackToPerform.AnimMontage != nullptr)
	{
		ActionDirection = MovementIntent;
		SetTurningRate(ActionTurnRate);
		SetMovementSpeed(AttackToPerform.Speed);
		SetAcceleration(AttackToPerform.Acceleration);
		Owner->PlayAnimMontage(AttackToPerform.AnimMontage);
	}
}

void UAttackComponent::SelectAttack_Implementation(const FString& AttackType) {}

void UAttackComponent::Reset_Implementation()
{
	TimelineToPlay = nullptr;
	ResetMovementSpeed();
	ResetTurningRate();
	ResetAcceleration();
}


void UAttackComponent::AttackMovement_Implementation()
{
	if (TimelineToPlay != nullptr)
	{
		TimelineToPlay->PlayFromStart();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Timeline not created properly."))
	}
}

void UAttackComponent::LimitTurn_Implementation()
{
	StopTurning();
}

void UAttackComponent::ResetTurn_Implementation()
{
	ResetTurningRate();
}

void UAttackComponent::SetFlyable_Implementation()
{
	Owner->GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Flying;
}

void UAttackComponent::ResetFlyable_Implementation()
{
	Owner->GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Walking;
}

void UAttackComponent::FinalizeAttackDirection_Implementation(FVector MovementIntent)
{
	ActionDirection = MovementIntent;
}

void UAttackComponent::ApplyDamageEffect_Implementation(AActor* Source, AActor* OtherActor)
{
	if (OtherActor->GetClass()->ImplementsInterface(UDamageable::StaticClass()))
	{
		IDamageable::Execute_ApplyHitEffect(OtherActor, Source, AttackToPerform.Damage, AttackToPerform.HitEffect, AttackToPerform.AttackEffect);
	}
}
