// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "D1PlayerHUDWidget.generated.h"


struct FD1CharacterStat;

/**
 * 
 */
UCLASS()
class D1_API UD1PlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public :
	UD1PlayerHUDWidget(const FObjectInitializer& ObjectInitializer);

public :
	void UpdateStat(const FD1CharacterStat& BaseStat , const FD1CharacterStat& ModeifierStat);
	void UpdateHp(float NewCurrentHp);

protected:
	UPROPERTY(BlueprintReadOnly , meta = (BindWidget))
	TObjectPtr<class UD1HpBarWidget> WidgetHpBar;

	UPROPERTY(BlueprintReadOnly , meta = (BindWidget))
	TObjectPtr<class UD1CharacterStatWidget> WidgetCharacterStat;	
};
