// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "D1HpBarWidget.generated.h"

struct FD1CharacterStat;


/**
 * 
 */
UCLASS()
class D1_API UD1HpBarWidget : public UUserWidget
{
	GENERATED_BODY()

public :
	UD1HpBarWidget(const FObjectInitializer& ObjectInitializer);

public :
	void UpdateStat(const FD1CharacterStat& BaseStat , const FD1CharacterStat& ModeifierStat);
	void UpdateHp(float NewCurrentHp);

protected:
	UPROPERTY(BLueprintReadOnly , meta = (BindWidget))
	TObjectPtr<class UProgressBar> HpBar;

	float CurrentHp;
	float MaxHp;
};
