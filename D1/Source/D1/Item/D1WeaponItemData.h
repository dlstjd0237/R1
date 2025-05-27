// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/D1ItemData.h"
#include  "GameData/D1CharacterStat.h"
#include "D1WeaponItemData.generated.h"

/**
 * 
 */
UCLASS()
class D1_API UD1WeaponItemData : public UD1ItemData
{
	GENERATED_BODY()

public :
	UD1WeaponItemData()
	{
		Type = EItemType::Weapon;
	}

public :
	UPROPERTY(EditAnywhere , Category = Weapon)
	TSoftObjectPtr<class USkeletalMesh> WeaponMesh;

	UPROPERTY(EditAnywhere , Category = Weapon)
	FD1CharacterStat ModifierStat;

	
};
