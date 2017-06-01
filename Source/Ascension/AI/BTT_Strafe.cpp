// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "BTT_Strafe.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Enemy/Goblin.h"


UBTT_Strafe::UBTT_Strafe()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_Strafe::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::InProgress;
}

void UBTT_Strafe::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	StrafeElapsedTime += DeltaSeconds;

	if (StrafeElapsedTime >= StrafeDuration)
	{
		StrafeElapsedTime = 0.0f;

		BlackboardComponent->SetValueAsBool(WantsAttackKey.SelectedKeyName, true);

		OwnerComp.OnTaskFinished(this, EBTNodeResult::Succeeded);
	}
	else
	{
		AActor* Owner = OwnerComp.GetAIOwner()->GetPawn();
		AGoblin* Character = Cast<AGoblin>(Owner);

		AActor* Enemy = Cast<AActor>(BlackboardComponent->GetValueAsObject(EnemyKey.SelectedKeyName));
	}
}
