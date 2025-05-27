// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/D1GameInterface.h"
#include "D1GameMode.generated.h"

/**
 * 
 */
UCLASS()
class D1_API AD1GameMode : public AGameModeBase , public ID1GameInterface
{
	GENERATED_BODY()

public:
	AD1GameMode();

public :
	void OnPlayerScoreChanged(int32 NewPlayerScore) override;
	void AddPlayerScore(int32 AddPlayerScore) override;
	void OnPlayerDead() override;
	bool IsGameCleared() override;

protected:
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	int32 ClearScore;

	UPROPERTY(VisibleAnywhere , BlueprintReadOnly)
	int32 CurrentScore;

	UPROPERTY(VisibleAnywhere , BlueprintReadOnly)
	uint8 bIsCleared : 1;
};
