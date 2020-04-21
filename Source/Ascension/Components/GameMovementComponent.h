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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float DefaultSpeed;

	/** Acceleration of the character. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float DefaultAcceleration;

	/** Base turn rate of the character. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float DefaultTurnRate;

	/** Direction the entity wants to move in. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector MovementDirection;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	/*
	 * Function to setup variables for controlled movement.
	 * @param TargetSpeed			Speed at which movement is to be performed.
	 * @param TargetAcceleration	Acceleration with which movement is to be performed.
	 * @param TargetTurnRate		Turn rate at which movement is to be performed.
	 */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual int SetupControlledMovement(float TargetSpeed, float TargetAcceleration, float TargetTurnRate);

	/*
	 * Function to setup variables for controlled movement during an ability.
	 * @param AbilityName	Name of the active ability.
	 */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual int SetupControlledMovementAbility(FString AbilityName);

	/*
	 * Function to perform controlled movement.
	 * @param MovementVector	Direction with which the entity should move.
	 */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ControlledMove(FVector MovementVector);

	/*
	 * Function that resets movement params to default values when controlled movement completes.
	 * @param InstanceID	ID of the instance of controlled movement.
	 */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual void FinishControlledMovement(int InstanceID);

	/*
	 * Function that resets movement params to default values when controlled movement completes.
	 * @param AbilityName	Name of the ability for which controlled movement was performed.
	 */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual void FinishControlledMovementAbility(FString AbilityName);

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

	/** Sets the direction for the entity to move in. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetMovementDirection(FVector Direction);

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

protected:
	/*
	 * Variable to keep track of the instance of controlled movement.
	 * Necessary to ensure that there isn't a race to reset movement parameters when two instances of controlled
	 * movement coincide.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	int ControlledMovementInstanceID;

	/*
	 * Map of ability names to their movement instance IDs.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	TMap<FString, int> AbilityNameIDMap;

};
