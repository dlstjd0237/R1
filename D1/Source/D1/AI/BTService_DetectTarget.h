// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_DetectTarget.generated.h"

/**
 * 
 */
UCLASS()
class D1_API UBTService_DetectTarget : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_DetectTarget();

public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp , uint8* NodeMemory , float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere)
	float DetectRange = 500.0f;
};
