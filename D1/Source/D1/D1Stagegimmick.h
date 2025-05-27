// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "D1Stagegimmick.generated.h"


UENUM(BlueprintType)
enum class EStageState : uint8
{
	READY = 0 ,
	FIGHT ,
	REWARD ,
	NEXT
};

DECLARE_DELEGATE(FOnStateChangedDelegate)

UCLASS()

class D1_API AD1Stagegimmick : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AD1Stagegimmick();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;


#pragma region

public :
	UFUNCTION()
	void OnStageTriggerBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent , AActor* OtherActor ,
	                                   UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep ,
	                                   const FHitResult& SweepResult);

protected:
	UPROPERTY(VisibleAnywhere , Category = Stage)
	TObjectPtr<class UStaticMeshComponent> StageMesh;

	UPROPERTY(VisibleAnywhere , Category = Stage)
	TObjectPtr<class UBoxComponent> StageTriggerBox;

#pragma endregion


#pragma region Gate

public:
	UFUNCTION()
	void OnGateTriggerBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent , AActor* OtherActor ,
	                                  UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep ,
	                                  const FHitResult& SweepResult);

	void OpenAllGates();
	void CloseAllGates();

protected:
	UPROPERTY(VisibleAnywhere , Category = Gate)
	TMap<FName , TObjectPtr<class UStaticMeshComponent>> GateMeshes;

	UPROPERTY(VisibleAnywhere , Category = Gate)
	TArray<TObjectPtr<class UBoxComponent>> GateTriggerBoxes;

#pragma endregion


#pragma  region GameState

public :
	void SetState(EStageState InNameState);
	void SetReady();
	void SetFight();
	void SetChooseReward();
	void SetChooseNext();

protected:
	UPROPERTY(EditAnywhere , Category = Stage)
	EStageState CurrentState;

	TMap<EStageState , FOnStateChangedDelegate> StateChangeActions;

#pragma  endregion

#pragma region FightState

public :
	UFUNCTION()
	void OnMonsterSpawn();

	UFUNCTION()
	void OnMonsterDestroyed(AActor* DestoryedActor);

protected:
	UPROPERTY(EditAnywhere , Category = Fight)
	TSubclassOf<class AD1Monster> MonsterClass;

	UPROPERTY(EditAnywhere , Category = Fight)
	float MonsterSpawnTime = 2.0f;

	FTimerHandle MonsterSpawnTimerHandle;
#pragma endregion


#pragma region RewardState

public :
	UFUNCTION()
	void SpawnRewardBoxes();

	UFUNCTION()
	void OnRewardBoxBegionOverlap(UPrimitiveComponent* OverlappedComponent , AActor* OtherActor ,
	                              UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep ,
	                              const FHitResult& SweepResult);

protected:
	UPROPERTY(VisibleAnywhere , Category = Reward)
	TArray<TWeakObjectPtr<class AD1ItemBox>> RewardBoxes;

	TMap<FName , FVector> RewardBoxLocations;

#pragma endregion


#pragma region StageStat

public :
	FORCEINLINE int32 GetStageLevel() { return CurrentStageLevel; }
	FORCEINLINE void SetStageLevel(int32 NewStageLevel) { CurrentStageLevel = NewStageLevel; }

protected:
	UPROPERTY(VisibleAnywhere , Category = Stage)
	int32 CurrentStageLevel = 0;
#pragma endregion
};
