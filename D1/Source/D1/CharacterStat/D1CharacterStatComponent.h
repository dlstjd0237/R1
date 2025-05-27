// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include  "GameData/D1CharacterStat.h"
#include "D1CharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate , float /*CurrentHP*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatChangedDelegate , const FD1CharacterStat& /*BaseStat*/,
                                     const FD1CharacterStat& /*ModifierStat*/);


UCLASS(ClassGroup=(Custom) , meta=(BlueprintSpawnableComponent))
class D1_API UD1CharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UD1CharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:
	float ApplyDamage(float InDamage);

public :
	FORCEINLINE float GetCurrentLevel() { return CurrentLevel; }
	FORCEINLINE float GetCurrentHp() { return CurrentHp; }
	FORCEINLINE const FD1CharacterStat& GetBaseStat() { return BaseStat; }
	FORCEINLINE const FD1CharacterStat& GetModifierStat() { return ModifierStat; }
	FORCEINLINE FD1CharacterStat GetTotalStat() const { return BaseStat + ModifierStat; }

public :
	void SetLevel(int32 NewLevel);
	void SetHp(float NewHp);
	FORCEINLINE void SetBaseStat(const FD1CharacterStat& InBaseStat)
	{
		BaseStat = InBaseStat;
		OnStatChanged.Broadcast(GetBaseStat() , GetModifierStat());
	}

	FORCEINLINE void SetModifierStat(const FD1CharacterStat& InModifierStat)
	{
		ModifierStat = InModifierStat;
		OnStatChanged.Broadcast(GetBaseStat() , GetModifierStat());
	}

	FORCEINLINE void AddBaseStat(const FD1CharacterStat& InAddBaseStat)
	{
		BaseStat = BaseStat + InAddBaseStat;
		OnStatChanged.Broadcast(GetBaseStat() , GetModifierStat());
	}

	FORCEINLINE void AddModifierStat(const FD1CharacterStat& InAddModifierStat)
	{
		ModifierStat = ModifierStat + InAddModifierStat;
		OnStatChanged.Broadcast(GetBaseStat() , GetModifierStat());
	}

public :
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;
	FOnStatChangedDelegate OnStatChanged;

protected:
	UPROPERTY(VisibleAnywhere , Category = Stat)
	float CurrentLevel;

	UPROPERTY(VisibleAnywhere , Category = Stat)
	float CurrentHp;

	UPROPERTY(VisibleAnywhere , Category = Stat)
	FD1CharacterStat BaseStat;

	UPROPERTY(VisibleAnywhere , Category = Stat)
	FD1CharacterStat ModifierStat;
};
