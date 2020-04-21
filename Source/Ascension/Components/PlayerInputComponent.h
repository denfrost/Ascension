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

	/*
	 * Constructor for the input action.
	 * @param Name			Name of the input action.
	 * @param Active		Whether the action is active.
	 * @param StartTime		Start time of the action (in seconds).
	 * @param EndTime		End time of the action (in seconds).
	 */
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

	/*
	 * Function to get the duration of the input action.
	 * @returns float	How long the input action has been active.
	 */
	FORCEINLINE float GetDuration() const
	{
		return EndTime - StartTime;
	}

};

/*
 * Struct representing a sequence of input actions.
 */
USTRUCT(BlueprintType)
struct FInputActionSequence
{
	GENERATED_BODY()

	/*
	 * Constructor for the input sequence.
	 */
	FInputActionSequence() {}

	/*
	 * Constructor for the input sequence.
	 * @param Actions	Input action sequence.
	 */
	FInputActionSequence(TArray<FInputAction> Actions)
		: InputActionSequence(Actions)
	{}

	/** Sequence on input actions for this event to be triggered. */
	UPROPERTY(VisibleAnywhere)
	TArray<FInputAction> InputActionSequence;

	/*
	 * Function to get the start time of the input sequence.
	 * @returns float	Start time of the sequence.
	 */
	FORCEINLINE float GetStartTime() const
	{
		if (InputActionSequence.Num() > 0)
		{
			return InputActionSequence[0].StartTime;
		}

		return 0.0f;
	}

	/*
	 * Function to get the end time of the input sequence.
	 * @returns float	End time of the sequence.
	 */
	FORCEINLINE float GetEndTime() const
	{
		if (InputActionSequence.Num() > 0)
		{
			int LastIndex = InputActionSequence.Num() - 1;
			return InputActionSequence[LastIndex].EndTime;
		}

		return 0.0f;
	}

	/*
	 * Function to get the duration of the input sequence.
	 * @returns float	Total duration of the sequence.
	 */
	FORCEINLINE float GetDuration() const
	{
		if (InputActionSequence.Num() > 0)
		{
			return (GetEndTime() - GetStartTime());
		}

		return 0.0f;
	}

	/*
	 * Function to print the contents of the input sequence.
	 */
	FORCEINLINE void Print() const
	{
		FString StatementToPrint = FString("Input sequence: ");

		for (int Index = 0; Index < InputActionSequence.Num(); Index++)
		{
			StatementToPrint = StatementToPrint.Append(InputActionSequence[Index].Name);
			StatementToPrint = StatementToPrint.Append(" | ");
		}

		UE_LOG(LogTemp, Warning, TEXT("%s"), *StatementToPrint)
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
		: Name(FString(""))
		, MinDuration(0.0f)
		, MaxDuration(0.0f)
		, MinInterval(0.0f)
		, MaxInterval(0.0f)
	{}

	/*
	 * Constructor for the action event.
	 * @param Name				Name of the action event.
	 * @param InputSequence		Sequence of input actions to trigger the event.
	 * @param ActionActiveMap	Map indicating whether input actions need to be active.
	 * @param MinDuration		Minimum duration the input actions should have lasted.
	 * @param MaxDuration		Maximum duration the input actions should have lasted.
	 * @param MinInterval		Minimum interval between input actions.
	 * @param MaxInterval		Maximum interval between input actions.
	 */
	FActionEvent(FString Name, TArray<FString> InputSequence, TMap<FString, bool> ActionActiveMap,
				 float MinDuration = 0.0f, float MaxDuration = 0.0f, float MinInterval = 0.0f,
				 float MaxInterval = 0.0f)
		: Name(Name)
		, InputSequence(InputSequence)
		, ActionActiveMap(ActionActiveMap)
		, MinDuration(MinDuration)
		, MaxDuration(MaxDuration)
		, MinInterval(MinInterval)
		, MaxInterval(MaxInterval)
	{}

	/** Name of the action event. */
	UPROPERTY(EditAnywhere)
	FString Name;

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
	 * @param Duration	Duration to check for.
	 * @returns bool	Whether the duration is valid.
	 */
	FORCEINLINE bool CheckDuration(float Duration) const
	{
		if ((MinDuration != 0.0f && Duration < MinDuration) ||
			(MaxDuration != 0.0f && Duration > MaxDuration))
		{
			return false;
		}

		return true;
	}

	/*
	 * Function to check whether the interval between input actions lies within acceptable ranges.
	 * @param Interval	Interval to check for.
	 * @returns bool	Whether the interval is valid.
	 */
	FORCEINLINE bool CheckInterval(float Interval) const
	{
		if ((MaxInterval != 0.0f && Interval > MaxInterval) ||
			(MinInterval != 0.0f && Interval < MinInterval))
		{
			return false;
		}

		return true;
	}

	/*
	 * Function to check whether an input action sequence is valid for this action event.
	 * @param SequenceToCompare		Input sequence to compare with.
	 * @returns bool				Whether the sequence is valid.
	 */
	FORCEINLINE bool CheckSequenceValidity(const FInputActionSequence& SequenceToCompore) const
	{
		TArray<FInputAction> InputActionsToCompare = SequenceToCompore.InputActionSequence;

		if (InputActionsToCompare.Num() == InputSequence.Num())
		{
			// Check whether any inputs that are required to be active/inactive are present in the correct state.
			for (FInputAction InputAction : InputActionsToCompare)
			{
				if (ActionActiveMap.Contains(InputAction.Name))
				{
					if (ActionActiveMap[InputAction.Name] != InputAction.Active)
					{
						return false;
					}
				}
			}

			// Check whether duration of each action is within the limits.
			for (int Index = 0; Index < InputActionsToCompare.Num(); Index++)
			{
				float Duration = InputActionsToCompare[Index].EndTime - InputActionsToCompare[Index].StartTime;
				if (!CheckDuration(Duration))
				{
					return false;
				}

				// Check whether intervals between actions is within the limits.
				if ((Index + 1) < InputActionsToCompare.Num())
				{
					float Interval = InputActionsToCompare[Index + 1].StartTime - InputActionsToCompare[Index].EndTime;
					if (!CheckInterval(Interval))
					{
						return false;
					}
				}
			}
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
	 * Constructor for the player input component.
	 */
	UPlayerInputComponent();

	/**
	 * Tick function for the input component. Updates the input buffer by removing inputs that aren't valid anymore.
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
	 * If the buffer is full, deletes the oldest input action that is not active and adds the new action to the end.
	 * @param InputAction	Input action to add.
	 */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void AddToBuffer(FInputAction& InputAction);

	/*
	 * Method to clear the input buffer.
	 */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void ClearBuffer();

	/*
	 * Method to get the last input action of the specified name in the input buffer.
	 * @param ActionEvent		Name of input action.
	 * @returns FInputAction	Input action with the given name.
	 */
	UFUNCTION(BlueprintCallable, Category = "Input")
	FInputAction GetLastInputAction(const FString& Name);

	/*
	 * Method to get the index of last input action of the specified name in the input buffer.
	 * @param ActionEvent	Name of input action.
	 * @returns int			Index of the last input action with the given name. -1 if the action was not found.
	 */
	UFUNCTION(BlueprintCallable, Category = "Input")
	int GetLastInputActionIndex(const FString& Name);

	/*
	 * Method to update the last input action of the specified name in the input buffer.
	 * @param ActionEvent	Name of input action.
	 * @param Active		Whether the action is active.
	 * @param EndTime		Time that the action ended/current time.
	 * @returns bool		Whether the action was updated successfully.
	 */
	UFUNCTION(BlueprintCallable, Category = "Input")
	bool UpdateLastInputAction(const FString& Name, const bool& Active, const float& EndTime);

	/*
	 * Method to try to add an input to the buffer.
	 * @param Name		Name of the InputAction to buffer.
	 * @param Active	Whether the action is going to be persistent.
	 */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void BufferInput(const FString& Name, bool Active);

	/*
	 * Method to get all sequences in the array which match an action event.
	 * @param Name								Name of the InputAction to buffer.
	 * @returns TArray<FInputActionSequence>	Array of sequences that match the event present in the buffer.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Input")
	TArray<FInputActionSequence> GetValidInputSequences(const FActionEvent& ActionEvent) const;

	/*
	 * Method to try and execute a buffered action.
	 * The first input sequence that completed successfully will be considered for execution.
	 * Clears the input buffer after an action is successfully executed.
	 * @returns bool	Whether an action was executed.
	 */
	UFUNCTION(BlueprintCallable, Category = "Input")
	bool TryBufferedAction();
	
	/*
	 * Method to print the contents of the input buffer.
	 */
	UFUNCTION(BlueprintCallable, Category = "Debug")
	void PrintBuffer();

};
