// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_AttackInRange.generated.h"

/**
 * 
 */
UCLASS()
class D1_API UBTDecorator_AttackInRange : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_AttackInRange();

public:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp , uint8* NodeMemory) const override;

protected:
	UPROPERTY(EditAnywhere)
	float AttackRange = 100.0f;
};
