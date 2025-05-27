// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "D1GameSingleton.generated.h"

struct FD1CharacterStat;

/**
 * 
 */
UCLASS()
class D1_API UD1GameSingleton : public UObject
{
	GENERATED_BODY()

public:
	UD1GameSingleton();
	static UD1GameSingleton& Get();

public :
	FD1CharacterStat GetCharacterStat(int32 InLevel) const;
	FORCEINLINE int32 GetCharacterMaxLevel() { return CharacterMaxLevel; }

private:
	TArray<FD1CharacterStat> CharacterStatTable;

	int32 CharacterMaxLevel;
};
