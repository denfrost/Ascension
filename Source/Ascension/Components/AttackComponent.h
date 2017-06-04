// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Classes/Components/TimelineComponent.h"
#include "Globals.h"
#include "AttackComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASCENSION_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttackComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:
	/** Speed at which the character walks. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed;

	/** Normal speed of the character. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalSpeed;

	/** Speed at which the character runs. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed;

	/** Acceleration of the character. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalAcceleration;

	/** Base turn rate of the charater. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalTurnRate;

	/** Turn rate of the character when performing an action. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float ActionTurnRate;

	/** Meter tracking the character's attack combo count. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	int ComboMeter;

	/** The direction the character should perform an action. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector ActionDirection;

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Initialization")
	void CreateAttack(const FString& AttackName, const FAttack& Attack);

public:
	/** Called for the character to perform the specified attack. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attack")
	void Attack(const FString& AttackName, const FVector& MovementIntent);

	/** Function to select the attack to perform next. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void SelectAttack(const FString& AttackType);

	/** Event called when a combo is finished/reset.
	* Performs necessary actions after a combo is completed.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void Reset();

	/** Plays the timeline for attack movement. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void AttackMovement();

public:
	/** Event called when rotation rate needs to be limited.
	* Limits rotation rate.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void LimitTurn();

	/** Event called when rotation rate needs to be reset.
	* Resets rotation rate.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void ResetTurn();

	/** Event called when character's movement needs to be set to flying.
	* Sets movement mode to flying.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void SetFlyable();

	/** Event called when character's movement needs to be set to walking.
	* Sets movement mode to walking.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void ResetFlyable();


	/** Finalizes character's attack direction. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void FinalizeAttackDirection(FVector MovementIntent);

	/** Function to apply an attack's effects to the hit actor. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void ApplyDamageEffect(AActor* Source, AActor* OtherActor);

protected:
	/** Called to stop character movement. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void StopMovement();

	/** Called to limit character movement to a certain speed. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetMovementSpeed(float Speed);

	/** Called to reset character movement to normal speed. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ResetMovementSpeed();

	/** Called to set acceleration to a value. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetAcceleration(float Acceleration);

	/** Called to reset acceleration. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ResetAcceleration();

	/** Called to stop character turning. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void StopTurning();

	/** Called to limit character turning to a certain rate. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetTurningRate(float Rate);

	/** Called to reset character turning to normal rate. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ResetTurningRate();

	/** Called to set gravity to a value. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetGravity(float GravityValue);

	/** Called to reset gravity. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ResetGravity();

protected:
	/** Function to handle the movement of the character in the timeline. */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void TimelineMovement(float Speed);

	/** A function that sets up the timeline for an attack move. */
	UFUNCTION(BlueprintCallable, Category = "Helper")
	void SetupTimelineComponent(UTimelineComponent* TimelineComponent, UCurveFloat* MovementCurve);


protected:
	/** Array containing the attacks. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
	TArray<FAttack> Attacks;

	/** Array containing movement timlines. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timelines")
	TArray<UTimelineComponent*> AttackTimelines;

	/** Structure that maps attack names to their respective indices. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
	TMap<FString, int> AttackNameMap;

protected:
	/** Attack to be performed. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animations")
	FAttack AttackToPerform;

	/** Used for comparison purposes. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animations")
	FAttack NullAttack;

protected:
	/** The timeline to be played. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	UTimelineComponent* TimelineToPlay;

private:
	/** The component's owner. */
	UPROPERTY(VisibleAnywhere, Category = "Owner")
	ACharacter* Owner;
};
