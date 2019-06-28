// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASCENSION_API UGameMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGameMovementComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	/** Normal speed of the character. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float DefaultSpeed;

	/** Acceleration of the character. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float DefaultAcceleration;

	/** Base turn rate of the character. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float DefaultTurnRate;

	/** Direction the entity wants to move in.. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector MovementIntent;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	/** Function to setup variables for precise motion. */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void SetupMovement(float TargetSpeed, float TargetAcceleration, float TargetTurnRate);

	/** Function that makes the character move in the specified direction. */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void Move(FVector BaseDirection, FVector MovementVector);

	/** Function that resets variables to normal values when the movement completes. */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void FinishMovement();

	/** Function that gets the direction the entity should move in.. */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void SetMovementIntent();

protected:
	/** Called to limit character movement to a certain speed. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetMovementSpeed(float Speed);

	/** Called to reset character movement to normal speed. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ResetMovementSpeed();

	/** Called to set acceleration to a value. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetAcceleration(float TargetAcceleration);

	/** Called to reset acceleration. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ResetAcceleration();

	/** Called to limit character turning to a certain rate. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetTurningRate(float Rate);

	/** Called to reset character turning to normal rate. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ResetTurningRate();

	/** Sets movement mode to flying. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetFlyable();

	/** Sets movement mode to walking. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ResetFlyable();

	/** Called to set gravity to a value. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetGravity(float GravityValue);

	/** Called to reset gravity. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ResetGravity();

public:
	/**
	 * Moves along the given movement direction using simple movement rules based on the current movement mode (usually used by simulated proxies).
	 *
	 * @param InVelocity:			Velocity of movement
	 * @param DeltaSeconds:			Time over which movement occurs
	 * @param OutStepDownResult:	[Out] If non-null, and a floor check is performed, this will be updated to reflect that result.
	 */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void PerformMove(const FVector& InVelocity, const float DeltaSeconds);
};
