// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/D1ItemData.h"
#include "D1PotionItemData.generated.h"

/**
 * 
 */
UCLASS()
class D1_API UD1PotionItemData : public UD1ItemData
{
	GENERATED_BODY()

public:
	UD1PotionItemData()
	{
		Type = EItemType::Potion;
	}

public :
	UPROPERTY(EditAnywhere , Category = Hp)
	float HealAmount;
};
