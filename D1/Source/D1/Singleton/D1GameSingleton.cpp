// Fill out your copyright notice in the Description page of Project Settings.


#include "Singleton/D1GameSingleton.h"

#include "GameData/D1CharacterStat.h"

UD1GameSingleton::UD1GameSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(
		TEXT("/Script/Engine.DataTable'/Game/GameData/DT_D1CharacterStat.DT_D1CharacterStat'"));

	if (DataTableRef.Succeeded())
	{
		const UDataTable* DataTable = DataTableRef.Object;

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray , CharacterStatTable ,
		                [](uint8* Value)
		                {
			                return *reinterpret_cast<FD1CharacterStat*>(Value);
		                }
		);
	}
	CharacterMaxLevel = CharacterStatTable.Num();
}

UD1GameSingleton& UD1GameSingleton::Get()
{
	UD1GameSingleton* Singleton = CastChecked<UD1GameSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}

	return *NewObject<UD1GameSingleton>();
}

FD1CharacterStat UD1GameSingleton::GetCharacterStat(int32 InLevel) const
{
	if (CharacterStatTable.IsValidIndex(InLevel-1))
	{
		return CharacterStatTable[InLevel-1];
	}else
	{
		return FD1CharacterStat();
	}
}
