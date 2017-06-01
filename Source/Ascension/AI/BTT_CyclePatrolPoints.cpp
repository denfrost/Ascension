// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "BTT_CyclePatrolPoints.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/Goblin.h"
#include "AIController.h"


EBTNodeResult::Type UBTT_CyclePatrolPoints::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	AActor* Owner = OwnerComp.GetAIOwner()->GetPawn();

	AGoblin* Character = Cast<AGoblin>(Owner);
	if (Character)
	{
		Character->PatrolPoints;
		int NextIndex = (BlackboardComponent->GetValueAsInt(PatrolPointIndexKey.SelectedKeyName) + 1) % Character->PatrolPoints.Num();

		BlackboardComponent->SetValueAsObject(PatrolPointKey.SelectedKeyName, Character->PatrolPoints[NextIndex]);
		BlackboardComponent->SetValueAsInt(PatrolPointIndexKey.SelectedKeyName, NextIndex);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
