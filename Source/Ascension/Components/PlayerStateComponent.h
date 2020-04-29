// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStateComponent.generated.h"


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
enum class EWeaponState : uint8
{
	WS_Sheathed			UMETA(DisplayName = "Sheathed"),
	WS_Unsheathed		UMETA(DisplayName = "Unsheathed")
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASCENSION_API UPlayerStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerStateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	/** General state that the character is in. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character State")
	ECharacterState CharacterState;

	/** State of character's movement. Indicates whether they are in ground/air. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character State")
	EMovementState MovementState;

	/** State that the character's weapon is in. Determines whether the weapon is sheathed/unsheathed. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character State")
	EWeaponState WeaponState;

public:
	/** Gets the character state. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Character State")
	ECharacterState GetCharacterState() const;

	/** Gets the movement state. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Character State")
	EMovementState GetMovementState() const;

	/** Gets the weapon state. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Character State")
	EWeaponState GetWeaponState() const;

	/** Gets the character state. */
	UFUNCTION(BlueprintCallable, Category = "State Helper")
	void SetCharacterState(ECharacterState State);

	/** Gets the movement state. */
	UFUNCTION(BlueprintCallable, Category = "State Helper")
	void SetMovementState(EMovementState State);

	/** Gets the weapon state. */
	UFUNCTION(BlueprintCallable, Category = "State Helper")
	void SetWeaponState(EWeaponState State);
	
protected:
	/** The component's owner. */
	UPROPERTY(VisibleAnywhere, Category = "Owner")
	AActor* Owner;
};
