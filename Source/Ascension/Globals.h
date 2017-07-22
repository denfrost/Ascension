#pragma once
#include "Ascension.h"
#include "Globals.generated.h"


UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	WS_Sheathed			UMETA(DisplayName="Sheathed"),
	WS_Unsheathed		UMETA(DisplayName="Unsheathed")
};

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	CS_Idle				UMETA(DisplayName = "Idle"),
	CS_Switching		UMETA(DisplayName = "Switching"),
	CS_Dodging			UMETA(DisplayName = "Dodging"),
	CS_Attacking		UMETA(DisplayName = "Attacking"),
	CS_Stunned			UMETA(DisplayName = "Stunned")
};

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	MS_OnGround			UMETA(DisplayName = "On Ground"),
	MS_InAir			UMETA(DisplayName = "InAir")
};

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
struct FAttack
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
