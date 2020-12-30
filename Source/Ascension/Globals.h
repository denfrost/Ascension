#pragma once
#include "Ascension.h"
#include "Globals.generated.h"


UENUM(BlueprintType)
enum class EHitEffect : uint8
{
	HE_PushBack			UMETA(DisplayName = "Push Back"),
	HE_LaunchUp			UMETA(DisplayName = "Launch Up"),
	HE_KnockBack		UMETA(DisplayName = "Knock Back")
};

UENUM(BlueprintType)
enum class EAIState : uint8
{
	AIS_Patrol			UMETA(DisplayName = "Patrol"),
	AIS_Combat			UMETA(DisplayName = "Combat")
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	ES_Idle				UMETA(DisplayName = "Idle"),
	ES_Dodging			UMETA(DisplayName = "Dodging"),
	ES_Attacking		UMETA(DisplayName = "Attacking")
};

UENUM(BlueprintType)
enum class EEnemyCombatState : uint8
{
	ECS_Observing		UMETA(DisplayName = "Observing"),
	ECS_Attacking		UMETA(DisplayName = "Attacking")
};


USTRUCT(BlueprintType)
struct FPlayerAnimation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UAnimMontage* AnimMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	float AnimSpeed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	float AnimAcceleration = 20000.0f;
};

USTRUCT(BlueprintType)
struct FAttackEffect
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	float KnockbackForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	float ZForce;
};

USTRUCT(BlueprintType)
struct FAttackStruct
{
	GENERATED_BODY()

	// Animation montage of the attack.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UAnimMontage* AnimMontage = nullptr;

	// Speed of movement during attack.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	float Speed = 0.0f;

	// Max acceleration during this attack.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	float Acceleration = 20000.0f;

	// Type of effect on the enemy (Push back, knock back etc.)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	EHitEffect HitEffect;

	// Damage dealt by the attack.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	float Damage;

	// Effect of the attack on the enemy.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	FAttackEffect AttackEffect;
};

/*
 * Struct for custom movement parameters.
 */
USTRUCT(BlueprintType)
struct FCustomMovementParams
{
	GENERATED_USTRUCT_BODY()

	// Speed of movement during attack.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Speed = 1000.0f;

	// Max acceleration during this attack.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Acceleration = 20000.0f;

	// Turn rate during the attack.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float TurnRate = 2048.0f;

	// Maximum angle the character can rotate during the attack.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxTurnAngleDegrees = 0.0f;

	// Whether the attack has Z-movement.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool HasZMovement = false;
};
