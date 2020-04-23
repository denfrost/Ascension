// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "Components/PlayerAttackComponent.h"
#include "Components/PlayerDodgeComponent.h"
#include "Entities/Characters/Player/AscensionPlayerController.h"
#include "PlayerInputComponent.h"


UPlayerInputComponent::UPlayerInputComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	BufferSize = 20;
	InputValidity = 0.5f;
	InactiveInputInterval = 0.1f;
	InputBuffer.Reset(BufferSize);
}

void UPlayerInputComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
										  FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	float CurrentTime = GetWorld()->GetTimeSeconds();

	// Process the normal input buffer.
	for (int Index = 0; Index < InputBuffer.Num(); Index++)
	{
		// Marking any inputs that aren't being updated anymore as inactive.
		if (((CurrentTime - InputBuffer[Index].EndTime) > InactiveInputInterval) &&
			(InputBuffer[Index].Active))
		{
			UE_LOG(LogInputBuffer, Warning, TEXT("Marking input %s as inactive."), *(InputBuffer[Index].Name))
			InputBuffer[Index].Active = false;
		}

		// Remove any input from the buffer if it isn't valid anymore.
		if ((CurrentTime - InputBuffer[Index].StartTime) > InputValidity)
		{
			if (!(InputBuffer[Index].Active))
			{
				UE_LOG(LogInputBuffer, Warning, TEXT("Removing input %s from the buffer."), *(InputBuffer[Index].Name))
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

void UPlayerInputComponent::AddToBuffer(FInputAction& InputAction)
{
	if ((BufferSize != 0) && (InputBuffer.Num() == BufferSize))
	{
		// Only remove the earliest inactive input.
		int IndexToRemove = 0;

		for (int Index = 0; Index < InputBuffer.Num(); Index++)
		{
			if (!(InputBuffer[Index].Active))
			{
				IndexToRemove = Index;
				break;
			}
		}

		InputBuffer.RemoveAt(IndexToRemove);
	}
	
	InputBuffer.Add(InputAction);
}

void UPlayerInputComponent::ClearBuffer()
{
	UE_LOG(LogInputBuffer, Warning, TEXT("Clearing input buffer."))
	InputBuffer.Reset(BufferSize);
}

FInputAction UPlayerInputComponent::GetLastInputAction(const FString& Name = FString(""))
{
	// If an empty string is passed, return the last input action.
	int LastIndex = GetLastInputActionIndex(Name);
	if (LastIndex >= 0)
	{
		return InputBuffer[LastIndex];
	}

	return FInputAction();
}

int UPlayerInputComponent::GetLastInputActionIndex(const FString& Name = FString(""))
{
	// If an empty string is passed, return the last input action.
	if (Name.Equals(FString("")))
	{
		int LastIndex = InputBuffer.Num() - 1;
		return LastIndex;
	}

	// Get the last input action based on the name.
	for (int Index = InputBuffer.Num() - 1; Index >= 0; Index--)
	{
		if (InputBuffer[Index].Name == Name)
		{
			return Index;
		}
	}

	return -1;
}

bool UPlayerInputComponent::UpdateLastInputAction(const FString& Name, const bool& Active, const float& EndTime)
{
	// If an empty string is passed, return the last input action.
	int LastIndex = GetLastInputActionIndex(Name);

	if (LastIndex >= 0)
	{
		FInputAction InputAction = InputBuffer[LastIndex];

		// We only want to update active input actions.
		if (InputAction.Active)
		{
			InputAction.Active = Active;
			InputAction.EndTime = EndTime;
			InputBuffer[LastIndex] = InputAction;

			return true;
		}
	}

	return false;
}

void UPlayerInputComponent::BufferInput(const FString& Name, bool Active)
{
	float CurrentTime = GetWorld()->GetTimeSeconds();

	if (!UpdateLastInputAction(Name, Active, CurrentTime))
	{
		FInputAction InputAction = FInputAction(Name, Active, CurrentTime, CurrentTime);
		UE_LOG(LogInputBuffer, Warning, TEXT("Adding input: %s"), *(InputAction.Name))
		AddToBuffer(InputAction);
		PrintBuffer();
	}
}

TArray<FInputActionSequence> UPlayerInputComponent::GetValidInputSequences(const FActionEvent& ActionEvent) const
{
	TArray<FInputActionSequence> ValidInputSequences = TArray<FInputActionSequence>();
	TMap<FString, TArray<int>> InputActionIndexMap = TMap<FString, TArray<int>>();

	TArray<FString> SequenceToMatch = ActionEvent.InputSequence;
	if (SequenceToMatch.Num() > 0)
	{
		// Store a mapping of each input action in the input buffer with the indices of that input action.
		for (int Index = 0; Index < InputBuffer.Num(); Index++)
		{
			FString InputName = InputBuffer[Index].Name;

			if (!InputActionIndexMap.Contains(InputName))
			{
				TArray<int> Indices = TArray<int>();
				Indices.Add(Index);
				InputActionIndexMap.Add(InputName, Indices);
			}
			else
			{
				InputActionIndexMap[InputName].Add(Index);
			}
		}

		if (InputActionIndexMap.Num() > 0)
		{
			FString FirstInput = SequenceToMatch[0];
			TArray<FInputActionSequence> MatchedInputSequences = TArray<FInputActionSequence>();

			if (InputActionIndexMap.Contains(FirstInput))
			{
				TArray<int> FirstInputIndices = InputActionIndexMap[FirstInput];
				TArray<TArray<int>> InputSequencesToExplore = TArray<TArray<int>>();

				// Each index of the first action is a sequence to explore.
				for (int Index : FirstInputIndices)
				{
					TArray<int> SequenceToExplore = TArray<int>();
					SequenceToExplore.Add(Index);
					InputSequencesToExplore.Add(SequenceToExplore);
				}

				// Find all input sequences in the buffer that match the required input sequence.
				// TODO: Rename variables for readability. Also possibly optimize this. Use bit operations?
				for (int Index = 0; Index < InputSequencesToExplore.Num(); Index++)
				{
					TArray<int> CurrentInputSequence = InputSequencesToExplore[Index];

					if (CurrentInputSequence.Num() < SequenceToMatch.Num())
					{
						// Get the indices of the next input action to match in the sequence.
						FString NextInput = SequenceToMatch[CurrentInputSequence.Num() + 1];

						if (InputActionIndexMap.Contains(NextInput))
						{
							TArray<int> NextInputIndices = InputActionIndexMap[NextInput];

							// Check whether the indices of the next input action is valid (in proper sequence).
							// TODO: Rename iterator variable for readability.
							for (int NextIndex = 0; NextIndex < NextInputIndices.Num(); NextIndex++)
							{
								int LastCurrentIndex = CurrentInputSequence.Num() - 1;
								if (NextInputIndices[NextIndex] > LastCurrentIndex)
								{
									TArray<int> NextSequenceToExplore = CurrentInputSequence;
									NextSequenceToExplore.Add(NextInputIndices[NextIndex]);

									// If the number of elements in the sequence is equal, then we have a match.
									if (NextSequenceToExplore.Num() == SequenceToMatch.Num())
									{
										// Add the sequence to the array of matched sequences.
										TArray<FInputAction> MatchedActions = TArray<FInputAction>();

										for (int SequenceIndex = 0; SequenceIndex < NextSequenceToExplore.Num(); SequenceIndex++)
										{
											MatchedActions.Add(InputBuffer[NextSequenceToExplore[SequenceIndex]]);
										}

										FInputActionSequence MatchedSequence = FInputActionSequence(MatchedActions);
										MatchedInputSequences.Add(MatchedSequence);
									}
									else
									{
										InputSequencesToExplore.Add(NextSequenceToExplore);
									}
								}
							}
						}
						
						// One of the input actions was not present in the buffer. In this case there is no need to explore further.
						else
						{
							break;
						}
					}

					// If the number of elements in the sequence is equal, then we have a match.
					else
					{
						// Add the sequence to the array of matched sequences.
						TArray<FInputAction> MatchedActions = TArray<FInputAction>();

						for (int SequenceIndex = 0; SequenceIndex < CurrentInputSequence.Num(); SequenceIndex++)
						{
							MatchedActions.Add(InputBuffer[CurrentInputSequence[SequenceIndex]]);
						}

						FInputActionSequence MatchedSequence = FInputActionSequence(MatchedActions);
						MatchedInputSequences.Add(MatchedSequence);
					}
				}

				// Check whether matched sequences are valid.
				for (int Index = 0; Index < MatchedInputSequences.Num(); Index++)
				{
					if (ActionEvent.CheckSequenceValidity(MatchedInputSequences[Index]))
					{
						ValidInputSequences.Add(MatchedInputSequences[Index]);
					}
				}
			}
		}
	}

	// Sort valid input sequences based on their end times.
	// TODO: Implement something better than selection sort.
	for (int OuterIndex = 0; OuterIndex < ValidInputSequences.Num() - 1; OuterIndex++)
	{
		for (int InnerIndex = OuterIndex + 1; InnerIndex < ValidInputSequences.Num(); InnerIndex++)
		{
			if (ValidInputSequences[InnerIndex].GetEndTime() < ValidInputSequences[OuterIndex].GetStartTime())
			{
				FInputActionSequence TempSequence = ValidInputSequences[OuterIndex];
				ValidInputSequences[OuterIndex] = ValidInputSequences[InnerIndex];
				ValidInputSequences[InnerIndex] = TempSequence;
			}
		}
	}

	return ValidInputSequences;
}

bool UPlayerInputComponent::TryBufferedAction()
{
	FActionEvent ActionEventToExecute = FActionEvent();

	// Kept as a pointer mainly for comparison/initialization in the loop.
	FInputActionSequence EarliestExecutedSequence = FInputActionSequence();

	for (FActionEvent ActionEvent : ActionEvents)
	{
		TArray<FInputActionSequence> ValidSequences = GetValidInputSequences(ActionEvent);

		if (ValidSequences.Num() > 0)
		{
			if (EarliestExecutedSequence.InputActionSequence.Num() != 0)
			{
				if (ValidSequences[0].GetEndTime() < EarliestExecutedSequence.GetEndTime())
				{
					ActionEventToExecute = ActionEvent;
					EarliestExecutedSequence = ValidSequences[0];
				}
			}
			else
			{
				// Since the valid sequences are sorted by their end times already, the first sequence will be the
				// one that was executed the earliest.
				ActionEventToExecute = ActionEvent;
				EarliestExecutedSequence = ValidSequences[0];
			}
		}
	}

	if (!ActionEventToExecute.Name.Equals(FString("")))
	{
		if (ActionEventToExecute.Name.Equals(FString("Light Attack")) || ActionEventToExecute.Name.Equals(FString("Strong Attack")))
		{
			AAscensionPlayerController* Controller = Cast<AAscensionPlayerController>(GetOwner());

			if (Controller)
			{
				APawn* Owner = Controller->GetPawn();

				if (Owner)
				{
					UPlayerAttackComponent* AttackComponent = Owner->FindComponentByClass<UPlayerAttackComponent>();

					if (AttackComponent)
					{
						bool Success = AttackComponent->Attack(ActionEventToExecute.Name);

						if (Success)
						{
							UE_LOG(LogInputBuffer, Warning, TEXT("Attack executed successfully."))
							ClearBuffer();
							PrintBuffer();
							return true;
						}
					}
				}
			}
			UE_LOG(LogInputBuffer, Warning, TEXT("Attack not executed."))
			PrintBuffer();
		}

		else if (ActionEventToExecute.Name.Equals(FString("Dodge")))
		{
			AAscensionPlayerController* Controller = Cast<AAscensionPlayerController>(GetOwner());

			if (Controller)
			{
				APawn* Owner = Controller->GetPawn();

				if (Owner)
				{
					UPlayerDodgeComponent* DodgeComponent = Owner->FindComponentByClass<UPlayerDodgeComponent>();

					if (DodgeComponent)
					{
						bool Success = DodgeComponent->Dodge(ActionEventToExecute.Name);

						if (Success)
						{
							UE_LOG(LogInputBuffer, Warning, TEXT("Dodge executed successfully."))
							ClearBuffer();
							PrintBuffer();
							return true;
						}
					}
				}
			}

			UE_LOG(LogInputBuffer, Warning, TEXT("Dodge not executed."))
			PrintBuffer();
		}
	}

	return false;
}

void UPlayerInputComponent::PrintBuffer()
{
	FString InputBufferContents = FString("Input buffer contents: ");

	for (FInputAction InputAction : InputBuffer)
	{
		InputBufferContents = InputBufferContents.Append(InputAction.Name);
		InputBufferContents = InputBufferContents.Append(FString(" | "));
	}

	UE_LOG(LogInputBuffer, Warning, TEXT("%s"), *InputBufferContents)
}
