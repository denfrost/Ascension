// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "AttackComponent.h"
#include "Interfaces/Damageable.h"


// Sets default values for this component's properties
UAttackComponent::UAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// Set movement speeds.
	WalkSpeed = 200.0f;
	NormalSpeed = 500.0f;
	SprintSpeed = 800.0f;
	NormalAcceleration = 2048.0f;
	NormalTurnRate = 540.0f;
	ActionTurnRate = 2048.0f;

	// Set gameplay variables.
	ComboMeter = 0;
	CanChain = false;

	// Setup the movement timelines.
	TimelineToPlay = nullptr;
	Light01Timeline = CreateDefaultSubobject<UTimelineComponent>(FName("Light01TimelineComponent"));
	Light02Timeline = CreateDefaultSubobject<UTimelineComponent>(FName("Light02TimelineComponent"));
	Light03Timeline = CreateDefaultSubobject<UTimelineComponent>(FName("Light03TimelineComponent"));
	Strong01Timeline = CreateDefaultSubobject<UTimelineComponent>(FName("Strong01TimelineComponent"));
	Strong02Timeline = CreateDefaultSubobject<UTimelineComponent>(FName("Strong02TimelineComponent"));
	Strong03Timeline = CreateDefaultSubobject<UTimelineComponent>(FName("Strong03TimelineComponent"));

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

	SetupTimelineComponent(Light01Timeline, LightAttack01.MovementCurve);
	SetupTimelineComponent(Light02Timeline, LightAttack02.MovementCurve);
	SetupTimelineComponent(Light03Timeline, LightAttack03.MovementCurve);
	SetupTimelineComponent(Strong01Timeline, StrongAttack01.MovementCurve);
	SetupTimelineComponent(Strong02Timeline, StrongAttack02.MovementCurve);
	SetupTimelineComponent(Strong03Timeline, StrongAttack03.MovementCurve);
}

void UAttackComponent::CreateAttack_Implementation(const FString& AttackName, const FAttack& Attack)
{
	//Attacks.Add(Attack);
	int AttackIndex = Attacks.Num() - 1;

	//UTimelineComponent* AttackTimeline = NewObject<UTimelineComponent>(this, FName(*AttackName));
	//SetupTimelineComponent(AttackTimeline, Attack.MovementCurve);
	//AttackTimelines.Add(AttackTimeline);

	AttackNameMap.Add(AttackName, AttackIndex);
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


void UAttackComponent::LightAttack(FVector MovementIntent)
{
	AttackToPerform = NullAttack;
	SelectAttack(FString("Light Attack"));

	if (AttackToPerform.AnimMontage != nullptr)
	{
		ActionDirection = MovementIntent;
		SetTurningRate(ActionTurnRate);
		SetMovementSpeed(AttackToPerform.Speed);
		SetAcceleration(AttackToPerform.Acceleration);
		Owner->PlayAnimMontage(AttackToPerform.AnimMontage);
	}
}

void UAttackComponent::StrongAttack(FVector MovementIntent)
{
	AttackToPerform = NullAttack;
	SelectAttack(FString("Strong Attack"));

	if (AttackToPerform.AnimMontage != nullptr)
	{
		ActionDirection = MovementIntent;
		SetTurningRate(ActionTurnRate);
		SetMovementSpeed(AttackToPerform.Speed);
		SetAcceleration(AttackToPerform.Acceleration);
		Owner->PlayAnimMontage(AttackToPerform.AnimMontage);
	}
}

void UAttackComponent::SelectAttack_Implementation(const FString& AttackType)
{
	if (AttackType.Equals(FString("Light Attack")))
	{
		switch (ComboMeter)
		{
		case 0:
			ComboMeter++;
			AttackToPerform = LightAttack01;
			if (TimelineToPlay != nullptr)
			{
				TimelineToPlay->Stop();
			}
			TimelineToPlay = Light01Timeline;
			break;

		case 1:
			ComboMeter++;
			AttackToPerform = LightAttack02;
			if (TimelineToPlay != nullptr)
			{
				TimelineToPlay->Stop();
			}
			TimelineToPlay = Light02Timeline;
			break;

		case 2:
			ComboMeter++;
			AttackToPerform = LightAttack03;
			if (TimelineToPlay != nullptr)
			{
				TimelineToPlay->Stop();
			}
			TimelineToPlay = Light03Timeline;
			break;

		default:
			ComboMeter = 1;
			AttackToPerform = LightAttack01;
			if (TimelineToPlay != nullptr)
			{
				TimelineToPlay->Stop();
			}
			TimelineToPlay = Light01Timeline;
			break;
		}
	}

	else if (AttackType.Equals(FString("Strong Attack")))
	{
		switch (ComboMeter)
		{
		case 0:
			ComboMeter++;
			AttackToPerform = StrongAttack01;
			if (TimelineToPlay != nullptr)
			{
				TimelineToPlay->Stop();
			}
			TimelineToPlay = Strong01Timeline;
			break;

		case 1:
			ComboMeter++;
			AttackToPerform = StrongAttack02;
			if (TimelineToPlay != nullptr)
			{
				TimelineToPlay->Stop();
			}
			TimelineToPlay = Strong02Timeline;
			break;

		case 2:
			ComboMeter++;
			AttackToPerform = StrongAttack03;
			if (TimelineToPlay != nullptr)
			{
				TimelineToPlay->Stop();
			}
			TimelineToPlay = Strong03Timeline;
			break;

		case 3:
			ComboMeter++;
			AttackToPerform = StrongAttack04;
			if (TimelineToPlay != nullptr)
			{
				TimelineToPlay->Stop();
			}
			break;

		default:
			ComboMeter = 1;
			AttackToPerform = StrongAttack01;
			if (TimelineToPlay != nullptr)
			{
				TimelineToPlay->Stop();
			}
			TimelineToPlay = Strong01Timeline;
			break;
		}
	}
}

void UAttackComponent::ResetCombo_Implementation()
{
	ComboMeter = 0;
	CanChain = false;
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
}

void UAttackComponent::LimitTurn_Implementation()
{
	StopTurning();
}

void UAttackComponent::ResetTurn_Implementation()
{
	ResetTurningRate();
}

void UAttackComponent::CanChainAttack_Implementation()
{
	CanChain = true;
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

void UAttackComponent::ApplySwordEffect(AActor* Source, AActor* OtherActor)
{
	if (OtherActor->GetClass()->ImplementsInterface(UDamageable::StaticClass()))
	{
		IDamageable::Execute_ApplyHitEffect(OtherActor, Source, AttackToPerform.Damage, AttackToPerform.HitEffect, AttackToPerform.AttackEffect);
	}
}
