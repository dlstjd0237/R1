// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/D1HpBarWidget.h"
#include "GameData/D1CharacterStat.h"
#include "Components/ProgressBar.h"

UD1HpBarWidget::UD1HpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1.0f;
}

void UD1HpBarWidget::UpdateStat(const FD1CharacterStat& BaseStat , const FD1CharacterStat& ModeifierStat)
{
	MaxHp = (BaseStat + ModeifierStat).MaxHP;

	if (HpBar)
	{
		HpBar->SetPercent(CurrentHp/MaxHp);
	}
}

void UD1HpBarWidget::UpdateHp(float NewCurrentHp)
{
	CurrentHp = NewCurrentHp;

	if (HpBar)
	{
		HpBar->SetPercent(CurrentHp/MaxHp);
	}
}
