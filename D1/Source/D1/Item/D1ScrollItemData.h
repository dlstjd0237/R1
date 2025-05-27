// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/D1ItemData.h"
#include "GameData/D1CharacterStat.h"
#include "D1ScrollItemData.generated.h"

/**
 * 
 */
UCLASS()
class D1_API UD1ScrollItemData : public UD1ItemData
{
	GENERATED_BODY()

public :
	UD1ScrollItemData()
	{
		Type = EItemType::Scroll;
	}

public:
	UPROPERTY(EditAnywhere , Category = Stat)
	FD1CharacterStat BaseStat;
	
};
