// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/InputComponent.h"
#include "PlayerInputComponent.generated.h"


/*
 * Struct representing an input action.
 */
USTRUCT(BlueprintType)
struct FInputAction
{
	GENERATED_BODY()

	/*
	 * Constructor for the input action.
	 */
	FInputAction()
		: Name(FString(""))
		, Active(false)
		, StartTime(0.0f)
		, EndTime(0.0f)
	{}

	/** Name of the action. */
	UPROPERTY(EditAnywhere)
	FString Name;

	/** Whether this event is currently active. */
	UPROPERTY(VisibleAnywhere)
	bool Active;

	/** Time that this action was triggered. */
	UPROPERTY(VisibleAnywhere)
	float StartTime;

	/** Time that this action ended. */
	UPROPERTY(VisibleAnywhere)
	float EndTime;

	FORCEINLINE float GetDuration() const
	{
		return EndTime - StartTime;
	}

};

/*
 * Struct representing an event to trigger based on a series of input actions.
 */
USTRUCT(BlueprintType)
struct FActionEvent
{
	GENERATED_BODY()

	/*
	 * Constructor for the action event.
	 */
	FActionEvent()
		: MinDuration(0.0f)
		, MaxDuration(0.0f)
		, MinInterval(0.0f)
		, MaxInterval(0.0f)
	{}

	/** Sequence on input actions for this event to be triggered. */
	UPROPERTY(EditAnywhere)
	TArray<FInputAction> InputSequence;

	/* Minimum duration of the input in order to be valid. If set to 0, this is not used. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, UIMin = 0))
	float MinDuration;

	/* Maximum duration of the input after which it is invalid. If set to 0, this is not used. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, UIMin = 0))
	float MaxDuration;

	/* Minimum time interval between input actions for them to be considered valid. If set to 0, this is not used. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, UIMin = 0))
	float MinInterval;

	/* Maximum time interval between input actions after which they are invalid. If set to 0, this is not used. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, UIMin = 0))
	float MaxInterval;

	FORCEINLINE bool CheckDuration(float Duration) const
	{
		if ((MinDuration != 0.f && Duration < MinDuration) ||
			(MaxDuration != 0.f && Duration > MaxDuration))
		{
			return false;
		}

		return true;
	}

	FORCEINLINE bool CheckInterval(float Interval) const
	{
		if ((MaxInterval != 0.f && Interval > MaxInterval) ||
			(MinInterval != 0.f && Interval < MinInterval))
		{
			return false;
		}

		return true;
	}

};

/*
 * Class handling player input.
 */
UCLASS()
class ASCENSION_API UPlayerInputComponent : public UInputComponent
{
	GENERATED_BODY()
	
public:
	/*
	 * A cyclic buffer for storing input actions.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Input")
	TArray<FInputAction> InputBuffer;

	/*
	 * A cyclic buffer for storing input actions that are persistent.
	 * If an input action is performed for longer than the input validity, it is removed from the input buffer
	 * and stored instead in the persistent input buffer.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Input")
	TArray<FInputAction> PersistentInputBuffer;

	/*
	 * A set of actions that can be performed based on input events.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	TArray<FActionEvent> ActionEvents;

	/** Time in seconds indicating how long a particular input is valid. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Input")
	float InputValidity;

	/** Amount of actions that can be stored in the input buffer. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	int BufferLength;

public:
	/*
	 * Method to add an input action.
	 * @param InputAction	Input action to add.
	 */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void AddInputAction(FInputAction& InputAction);

	/*
	 * Method to add an action event.
	 * @param ActionEvent	Action event to add.
	 */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void AddActionEvent(FActionEvent& ActionEvent);

};
