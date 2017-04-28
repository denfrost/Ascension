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
	CS_Attacking		UMETA(DisplayName = "Attacking")
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
	AIS_Patrolling		UMETA(DisplayName = "Patrolling"),
	AIS_Observing		UMETA(DisplayName = "Observing"),
	AIS_Attacking		UMETA(DisplayName = "Attacking")
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UCurveFloat* MovementCurve;
};

USTRUCT(BlueprintType)
struct FAttack
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UAnimMontage* AnimMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	float Speed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	float Acceleration = 20000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UCurveFloat* MovementCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	EHitEffect HitEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	float Damage;
};
