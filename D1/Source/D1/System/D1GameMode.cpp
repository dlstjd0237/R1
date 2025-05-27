// Fill out your copyright notice in the Description page of Project Settings.


#include "System/D1GameMode.h"
#include "Player/D1PlayerController.h"

AD1GameMode::AD1GameMode()
{
	ClearScore = 3;
	CurrentScore = 0;
	bIsCleared = false;
}

void AD1GameMode::OnPlayerScoreChanged(int32 NewPlayerScore)
{
	CurrentScore = NewPlayerScore;

	AD1PlayerController* D1PlayerController = Cast<AD1PlayerController>(GetWorld()->GetFirstPlayerController());
	if (D1PlayerController)
	{
		D1PlayerController->GameScoreChanged(CurrentScore);
	}
	if (CurrentScore >= ClearScore)
		bIsCleared = true;
}

void AD1GameMode::AddPlayerScore(int32 AddPlayerScore)
{
	CurrentScore += AddPlayerScore;
	AD1PlayerController* D1PlayerController = Cast<AD1PlayerController>(GetWorld()->GetFirstPlayerController());
	if (D1PlayerController)
	{
		D1PlayerController->GameScoreChanged(CurrentScore);
	}

	if (CurrentScore >= ClearScore)
	{
		bIsCleared = true;
		if (D1PlayerController)
		{
			D1PlayerController->GameClear();
		}
	}
}

void AD1GameMode::OnPlayerDead()
{
	AD1PlayerController* D1PlayerController = Cast<AD1PlayerController>(GetWorld()->GetFirstPlayerController());
	if (D1PlayerController)
	{
		D1PlayerController->GameOver();
	}
}

bool AD1GameMode::IsGameCleared()
{
	return bIsCleared;
}
