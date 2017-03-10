#pragma once


UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	WS_Sheathed		UMETA(DisplayName="Sheathed"),
	WS_Unsheathed		UMETA(DisplayName="Unsheathed")
};

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	CS_Idle				UMETA(DisplayName = "Idle"),
	CS_InAir			UMETA(DisplayName = "InAir"),
	CS_Switching		UMETA(DisplayName = "Switching"),
	CS_Dodging			UMETA(DisplayName = "Dodging"),
	CS_Attacking		UMETA(DisplayName = "Attacking")
};
