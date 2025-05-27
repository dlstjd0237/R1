// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "D1AIInterface.generated.h"

DECLARE_DELEGATE(FAIAttackFinished);


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UD1AIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class D1_API ID1AIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual float GetAttackRange() = 0;
	virtual void AttackByAI() = 0;
	virtual void SetAIAttackFinishedDelegate(const FAIAttackFinished& InOnAttackFinished) = 0;
};
