// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "D1PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class D1_API AD1PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AD1PlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

public :
	UFUNCTION(BlueprintImplementableEvent , Meta = (DIsplayName= "OnScoreChangedCpp"))
	void K2_OnScoreChanged(int32 NewScore);

	UFUNCTION(BlueprintImplementableEvent , Meta = (DIsplayName= "OnGameClearCpp"))
	void K2_GameClear();

	UFUNCTION(BlueprintImplementableEvent , Meta = (DIsplayName= "OnGameOverCpp"))
	void K2_GameOver();

	UFUNCTION(BlueprintImplementableEvent , Meta = (DIsplayName= "OnGameRetryCountCpp"))
	void K2_OnGameRetryCount(int32 NewRetryCount);


public :
	void GameScoreChanged(int32 NewScore);
	void GameClear();
	void GameOver();

protected:
	UPROPERTY(visibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UD1SaveGame> SaveGameInstance;
};
