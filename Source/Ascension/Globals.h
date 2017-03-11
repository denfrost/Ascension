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

USTRUCT(BlueprintType)
struct FPlayerAnimation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UAnimMontage* AnimMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	float LaunchForwardForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	float LaunchUpwardForce;
};
