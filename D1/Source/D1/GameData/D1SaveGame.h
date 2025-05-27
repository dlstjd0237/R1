// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "D1SaveGame.generated.h"

/**
 * 
 */
UCLASS()
class D1_API UD1SaveGame : public USaveGame
{
	GENERATED_BODY()
public :
	UD1SaveGame();

public :
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 RetryCount;
};
