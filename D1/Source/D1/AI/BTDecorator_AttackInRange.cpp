// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_AttackInRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/D1AIInterface.h"

class ID1AIInterface;

UBTDecorator_AttackInRange::UBTDecorator_AttackInRange()
{
	NodeName = TEXT("AttackInRange");
}

bool UBTDecorator_AttackInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp , uint8* NodeMemory) const
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
		return false;

	APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));
	if (nullptr == TargetPawn)
		return false;

	ID1AIInterface* D1AIPawn = Cast<ID1AIInterface>(ControllingPawn);
	if (D1AIPawn)
	{
		// AttackRange = (ControllingPawn->GetDistanceTo(TargetPawn) < -D1AIPawn->GetAttackRange());
	}
	

	return (ControllingPawn->GetDistanceTo(TargetPawn) <= AttackRange);
}
