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

	FInputAction(FString Name, bool Active, float StartTime, float EndTime)
		: Name(Name)
		, Active(Active)
		, StartTime(StartTime)
		, EndTime(EndTime)
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

	/** Time that this action ended. If the ability is still active, this would be the current time. */
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
	TArray<FString> InputSequence;

	/*
	 * Map indicating whether a particular input action needs to be active to be considered for triggering the event.
	 * If no entry is found for a particular action, it is assumed that the action need not have been active.
	 */
	UPROPERTY(EditAnywhere)
	TMap<FString, bool> ActionActiveMap;

	/** Minimum duration of the input in order to be valid. If set to 0, this is not used. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, UIMin = 0))
	float MinDuration;

	/** Maximum duration of the input after which it is invalid. If set to 0, this is not used. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, UIMin = 0))
	float MaxDuration;

	/** Minimum time interval between input actions for them to be considered valid. If set to 0, this is not used. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, UIMin = 0))
	float MinInterval;

	/** Maximum time interval between input actions after which they are invalid. If set to 0, this is not used. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, UIMin = 0))
	float MaxInterval;

	/*
	 * Function to check whether the duration of an input action lies within acceptable ranges.
	 */
	FORCEINLINE bool CheckDuration(float Duration) const
	{
		if ((MinDuration != 0.f && Duration < MinDuration) ||
			(MaxDuration != 0.f && Duration > MaxDuration))
		{
			return false;
		}

		return true;
	}

	/*
	 * Function to check whether the interval between input actions lies within acceptable ranges.
	 */
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
 * Enum depicting the type of input buffer.
 */
UENUM(BlueprintType)
enum class FBufferType : uint8
{
	BT_Normal = 0,
	BT_Persistent = 1
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
	 * Constructor for the player input component.
	 */
	UPlayerInputComponent();

	/**
	 * Tick function for the input component. Updates the input buffer by removing/moving inputs that aren't valid anymore.
	 * @param DeltaTime			The time since the last tick.
	 * @param TickType			The kind of tick this is, for example, are we paused, or 'simulating' in the editor.
	 * @param ThisTickFunction	Internal tick function struct that caused this to run.
	 */
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

public:
	/*
	 * A set of actions that can be performed based on input events.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	TArray<FActionEvent> ActionEvents;

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

	/** Amount of actions that can be stored in the input buffer. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	int BufferSize;

	/** Time in seconds indicating how long a particular input is valid. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	float InputValidity;

	/** Time in seconds indicating how long an input can be considered to be active. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	float InactiveInputInterval;

public:
	/*
	 * Method to add an action event.
	 * @param ActionEvent	Action event to add.
	 */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void AddActionEvent(FActionEvent& ActionEvent);

	/*
	 * Method to add an input action to the buffer.
	 * If the buffer is full, deletes the oldest input action and adds the new action to the end.
	 * @param InputAction	Input action to add.
	 * @param BufferType	Type of input buffer.
	 */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void AddToBuffer(FInputAction& InputAction, const FBufferType& BufferType);

	/*
	 * Method to clear the input buffer.
	 * @param BufferType	Type of input buffer.
	 */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void ClearBuffer(const FBufferType& BufferType);

	/*
	 * Method to get the last input action of the specified name in the input buffer.
	 * @param ActionEvent		Name of input action.
	 * @returns FInputAction	Input action with the given name.
	 * @param BufferType		Type of input buffer.
	 */
	UFUNCTION(BlueprintCallable, Category = "Input")
	FInputAction GetLastInputAction(const FString& Name, const FBufferType& BufferType);

	/*
	 * Method to get the index of last input action of the specified name in the input buffer.
	 * @param ActionEvent	Name of input action.
	 * @param BufferType	Type of input buffer.
	 * @returns int			Index of the last input action with the given name. -1 if the action was not found.
	 */
	UFUNCTION(BlueprintCallable, Category = "Input")
	int GetLastInputActionIndex(const FString& Name, const FBufferType& BufferType);

	/*
	 * Method to update the last input action of the specified name in the input buffer.
	 * @param ActionEvent	Name of input action.
	 * @param Active		Whether the action is active.
	 * @param EndTime		Time that the action ended/current time.
	 * @param BufferType	Type of input buffer.
	 * @returns bool		Whether the action was updated successfully.
	 */
	UFUNCTION(BlueprintCallable, Category = "Input")
	bool UpdateLastInputAction(const FString& Name, const bool& Active, const float& EndTime,
							   const FBufferType& BufferType);

	/*
	 * Method to try to add an input to the buffer.
	 * @param Name		Name of the InputAction to buffer.
	 * @param Active	Whether the action is going to be persistent.
	 */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void BufferInput(const FString& Name, bool Active);
	
	/*
	 * Method to print the contents of the input buffer.
	 * @param BufferType	Type of input buffer.
	 */
	UFUNCTION(BlueprintCallable, Category = "Debug")
	void PrintBuffer(const FBufferType& BufferType);

};
