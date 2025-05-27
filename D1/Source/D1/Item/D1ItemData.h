// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "D1ItemData.generated.h"

UENUM(BlueprintType)
enum class EItemType: uint8
{
	Weapon = 0 ,
	Potion ,
	Scroll
};

/**
 * 
 */
UCLASS()
class D1_API UD1ItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public :
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(FName("D1ItemData") , GetFName());
	}

public :
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = Type)
	EItemType Type;
};
