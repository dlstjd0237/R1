// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "D1CharacterStat.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FD1CharacterStat : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FD1CharacterStat() : MaxHP(0.0f), Attack(0.0f), AttackRange(0.0f),
	                     AttackRadius(0.0f), AttackSpeed(0.0f), MovementSpeed(0.0f)
	{
	}

public:
	FD1CharacterStat operator+(const FD1CharacterStat& Other) const
	{
		const float* const ThisPtr = reinterpret_cast<const float* const>(this);
		const float* const OtherPtr = reinterpret_cast<const float* const>(&Other);

		FD1CharacterStat Result;
		float* ResultPtr = reinterpret_cast<float*>(&Result);

		int32 StatNumber = sizeof(FD1CharacterStat) / sizeof(float);
		for (int32 i = 0; i < StatNumber; ++i)
		{
			ResultPtr[i] = ThisPtr[i] + OtherPtr[i];
		}
		return Result;
	}

public:
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = Stat)
	float MaxHP;

	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = Stat)
	float Attack;

	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = Stat)
	float AttackRange;

	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = Stat)
	float AttackRadius;

	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = Stat)
	float AttackSpeed;

	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = Stat)
	float MovementSpeed;
};
