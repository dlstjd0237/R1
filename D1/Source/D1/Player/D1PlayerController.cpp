// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/D1PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include  "GameData//D1SaveGame.h"

AD1PlayerController::AD1PlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AD1PlayerController::BeginPlay()
{
	Super::BeginPlay();
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	SaveGameInstance = Cast<UD1SaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Player0") , 0));
	if (SaveGameInstance)
	{
		SaveGameInstance->RetryCount++;
	}
	else
	{
		SaveGameInstance = NewObject<UD1SaveGame>();
		SaveGameInstance->RetryCount = 0;
	}

	K2_OnGameRetryCount(SaveGameInstance->RetryCount);
}

void AD1PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void AD1PlayerController::GameScoreChanged(int32 NewScore)
{
	K2_OnScoreChanged(NewScore);
}

void AD1PlayerController::GameClear()
{
	K2_GameClear();

	if (SaveGameInstance)
	{
		SaveGameInstance-> RetryCount = 0;
		if (!UGameplayStatics::SaveGameToSlot(SaveGameInstance,TEXT("Player0"),0))
		{
			UE_LOG(LogTemp,Error,TEXT("Save Game Error!"));
		}
	}
}

void AD1PlayerController::GameOver()
{
	K2_GameOver();

	if (!UGameplayStatics::SaveGameToSlot(SaveGameInstance,TEXT("Player0"),0))
	{
		UE_LOG(LogTemp,Error,TEXT("Save Game Error!"));
	}
}
