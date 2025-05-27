// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/D1CharacterBase.h"
#include "Interface/D1AIInterface.h"
#include "Engine/StreamableManager.h"
#include "D1Monster.generated.h"

/**
 *
 */
UCLASS(config = Monster)
class D1_API AD1Monster : public AD1CharacterBase , public ID1AIInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AD1Monster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;
	
public:
	virtual void SetDead() override;

	virtual float GetAttackRange() override;
	virtual void AttackByAI() override;
	virtual void SetAIAttackFinishedDelegate(const FAIAttackFinished& InOnAttackFinished) override;

	virtual void ComboAttackEnd(class UAnimMontage* TargetMontage , bool IsProperlyEnded) override;

	void MonsterMeshLoadCompleted();

protected:
	float DeadEventDelayTime = 5.0f;

	FAIAttackFinished OnAttackFinished;

protected:
	UPROPERTY(config)
	TArray<FSoftObjectPath> MonsterMeshes;

	TSharedPtr<FStreamableHandle> MonsterMeshHandle;
 };
