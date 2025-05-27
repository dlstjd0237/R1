// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"

#include "AIController.h"
#include "Interface/D1AIInterface.h"

UBTTask_Attack::UBTTask_Attack()
{
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp , uint8* NodeMemory)
{
	if (Super::ExecuteTask(OwnerComp , NodeMemory) == EBTNodeResult::Failed)
		return EBTNodeResult::Failed;

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
		return EBTNodeResult::Failed;

	//공격 구현
	ID1AIInterface* D1AIPawn = Cast<ID1AIInterface>(ControllingPawn);
	if (nullptr == D1AIPawn)
		return EBTNodeResult::Failed;

	FAIAttackFinished OnAttackFinished;
	OnAttackFinished.BindLambda(
		[&]()
		{
			FinishLatentTask(OwnerComp , EBTNodeResult::Succeeded);
		}
	);

	D1AIPawn->SetAIAttackFinishedDelegate(OnAttackFinished);
	D1AIPawn->AttackByAI();
	return EBTNodeResult::InProgress;
}
