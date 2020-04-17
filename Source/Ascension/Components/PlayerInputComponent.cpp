// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "PlayerInputComponent.h"


UPlayerInputComponent::UPlayerInputComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	BufferSize = 20;
	InputValidity = 0.5f;
	InactiveInputInterval = 0.1f;
	InputBuffer.Reset(BufferSize);
	PersistentInputBuffer.Reset(BufferSize);
}

void UPlayerInputComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
										  FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	float CurrentTime = GetWorld()->GetTimeSeconds();
	
	// Process the persistent input buffer.
	for (int Index = 0; Index < PersistentInputBuffer.Num(); Index++)
	{
		// Marking any persistent inputs that aren't being updated anymore as inactive.
		if (((CurrentTime - PersistentInputBuffer[Index].EndTime) > InactiveInputInterval) &&
			(PersistentInputBuffer[Index].Active))
		{
			UE_LOG(LogTemp, Warning, TEXT("Marking persistent input %s as inactive."), *(PersistentInputBuffer[Index].Name))
			PersistentInputBuffer[Index].Active = false;
		}

		// Remove any input from the buffer if it isn't valid anymore or move it to the persistent buffer if its still active.
		if (((CurrentTime - PersistentInputBuffer[Index].StartTime) > InputValidity) &&
			(!(PersistentInputBuffer[Index].Active)))
		{
			UE_LOG(LogTemp, Warning, TEXT("Removing input %s from the persistent buffer."), *(PersistentInputBuffer[Index].Name))
			PersistentInputBuffer.RemoveAt(Index);
			Index--;
		}
	}

	// Process the normal input buffer.
	for (int Index = 0; Index < InputBuffer.Num(); Index++)
	{
		// Marking any inputs that aren't being updated anymore as inactive.
		if (((CurrentTime - InputBuffer[Index].EndTime) > InactiveInputInterval) &&
			(InputBuffer[Index].Active))
		{
			UE_LOG(LogTemp, Warning, TEXT("Marking input %s as inactive."), *(InputBuffer[Index].Name))
			InputBuffer[Index].Active = false;
		}

		// Remove any input from the buffer if it isn't valid anymore or move it to the persistent buffer if its still active.
		if ((CurrentTime - InputBuffer[Index].StartTime) > InputValidity)
		{
			if (InputBuffer[Index].Active)
			{
				UE_LOG(LogTemp, Warning, TEXT("Moving input %s to the persistent buffer."), *(InputBuffer[Index].Name))
				FInputAction InputAction = InputBuffer[Index];
				InputBuffer.RemoveAt(Index);
				AddToBuffer(InputAction, FBufferType::BT_Persistent);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Removing input %s from the buffer."), *(InputBuffer[Index].Name))
				InputBuffer.RemoveAt(Index);
				Index--;
			}
		}
	}
}


void UPlayerInputComponent::AddActionEvent(FActionEvent& ActionEvent)
{
	ActionEvents.Add(ActionEvent);
}

void UPlayerInputComponent::AddToBuffer(FInputAction& InputAction,
										const FBufferType& BufferType = FBufferType::BT_Normal)
{
	// TODO: Handle the case where oldest active input would need to be transferred to the persistent input buffer.
	TArray<FInputAction>& Buffer = BufferType == FBufferType::BT_Normal ? InputBuffer : PersistentInputBuffer;
	if ((BufferSize != 0) && (Buffer.Num() == BufferSize))
	{
		Buffer.RemoveAt(0);
	}
	
	Buffer.Add(InputAction);
}

void UPlayerInputComponent::ClearBuffer(const FBufferType& BufferType = FBufferType::BT_Normal)
{
	TArray<FInputAction>& Buffer = BufferType == FBufferType::BT_Normal ? InputBuffer : PersistentInputBuffer;
	Buffer.Reset(BufferSize);
}

FInputAction UPlayerInputComponent::GetLastInputAction(const FString& Name = FString(""),
													   const FBufferType& BufferType = FBufferType::BT_Normal)
{
	// If an empty string is passed, return the last input action.
	int LastIndex = GetLastInputActionIndex(Name, BufferType);
	TArray<FInputAction>& Buffer = BufferType == FBufferType::BT_Normal ? InputBuffer : PersistentInputBuffer;
	if (LastIndex >= 0)
	{
		return Buffer[LastIndex];
	}

	return FInputAction();
}

int UPlayerInputComponent::GetLastInputActionIndex(const FString& Name = FString(""),
												   const FBufferType& BufferType = FBufferType::BT_Normal)
{
	TArray<FInputAction>& Buffer = BufferType == FBufferType::BT_Normal ? InputBuffer : PersistentInputBuffer;

	// If an empty string is passed, return the last input action.
	if (Name.Equals(FString("")))
	{
		int LastIndex = Buffer.Num() - 1;
		return LastIndex;
	}

	// Get the last input action based on the name.
	for (int Index = Buffer.Num() - 1; Index >= 0; Index--)
	{
		if (Buffer[Index].Name == Name)
		{
			return Index;
		}
	}

	return -1;
}

bool UPlayerInputComponent::UpdateLastInputAction(const FString& Name, const bool& Active, const float& EndTime,
												  const FBufferType& BufferType = FBufferType::BT_Normal)
{
	// If an empty string is passed, return the last input action.
	int LastIndex = GetLastInputActionIndex(Name, BufferType);
	TArray<FInputAction>& Buffer = BufferType == FBufferType::BT_Normal ? InputBuffer : PersistentInputBuffer;

	if (LastIndex >= 0)
	{
		FInputAction InputAction = Buffer[LastIndex];

		// We only want to update active input actions.
		if (InputAction.Active)
		{
			InputAction.Active = Active;
			InputAction.EndTime = EndTime;
			Buffer[LastIndex] = InputAction;

			return true;
		}
	}

	return false;
}

void UPlayerInputComponent::BufferInput(const FString& Name, bool Active)
{
	float CurrentTime = GetWorld()->GetTimeSeconds();

	if (!UpdateLastInputAction(Name, Active, CurrentTime, FBufferType::BT_Persistent))
	{
		if (!UpdateLastInputAction(Name, Active, CurrentTime))
		{
			FInputAction InputAction = FInputAction(Name, Active, CurrentTime, CurrentTime);
			UE_LOG(LogTemp, Warning, TEXT("Adding input: %s"), *(InputAction.Name))
			AddToBuffer(InputAction);
			PrintBuffer(FBufferType::BT_Normal);
		}
	}
}

void UPlayerInputComponent::PrintBuffer(const FBufferType& BufferType = FBufferType::BT_Normal)
{
	TArray<FInputAction>& Buffer = BufferType == FBufferType::BT_Normal ? InputBuffer : PersistentInputBuffer;
	FString InputBufferContents = FString("Input buffer contents: ");

	for (FInputAction InputAction : Buffer)
	{
		InputBufferContents = InputBufferContents.Append(InputAction.Name);
		InputBufferContents = InputBufferContents.Append(FString(" | "));
	}

	UE_LOG(LogTemp, Warning, TEXT("%s"), *InputBufferContents)
}
