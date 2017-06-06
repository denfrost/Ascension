// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "BTT_Attack.h"
#include "AIController.h"
#include "Enemy/Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/AttackComponent.h"


EBTNodeResult::Type UBTT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	BlackboardComponent = OwnerComp.GetBlackboardComponent();
	ACharacter* ControlledPawn = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetControlledPawn());
	AEnemy* Enemy = Cast<AEnemy>(ControlledPawn);

	Enemy->Attack();
	UAttackComponent* AttackComponent = Enemy->FindComponentByClass<UAttackComponent>();

	if (AttackComponent)
	{
		AttackComponent->OnAttackComplete.AddDynamic(this, &UBTT_Attack::AttackComplete);
	}

	return EBTNodeResult::InProgress;
}

void UBTT_Attack::AttackComplete_Implementation(const bool Successful)
{
	BlackboardComponent->SetValueAsBool(WantsObserveKey.SelectedKeyName, true);
}
