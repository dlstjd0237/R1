// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TurnToTarget.generated.h"

/**
 * 
 */
UCLASS()
class D1_API UBTTask_TurnToTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_TurnToTarget();

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp , uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp , uint8* NodeMemory , float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere)
	float TurnSpeed = 0.0f;

	FRotator CurrentRot;
	FRotator TargetRot;
	
};
