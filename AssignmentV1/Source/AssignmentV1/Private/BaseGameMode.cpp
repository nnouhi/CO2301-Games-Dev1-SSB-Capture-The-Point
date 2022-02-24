// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameMode.h"


int32 ABaseGameMode::Difficulty = 1;
int32 ABaseGameMode::PreviousLevelScore = 0;
float ABaseGameMode::BaseTurnRate = 45.f;

void ABaseGameMode::ProgressNextLevel()
{
	return;
}

FString ABaseGameMode::GetObjectiveMessage()
{
	return TEXT("");
}

FString ABaseGameMode::GetLevelName()
{
	return TEXT("");
}

void ABaseGameMode::OnLevelComplete()
{
	ProgressNextLevel();
	return;
}

int32 ABaseGameMode::GetScore()
{
	return 0;
}

int32 ABaseGameMode::GetRemainingEnemies()
{
	return 0;
}

int32 ABaseGameMode::GetTotalEnemies()
{
	return 0;
}

FString ABaseGameMode::GetTimeRemaining()
{
	return TEXT("");
}

FString ABaseGameMode::GetPlayersFlagPercentage()
{
	return TEXT("");
}

FString ABaseGameMode::GetAIsFlagPercentage()
{
	return TEXT("");
}

void ABaseGameMode::SetDifficulty(int32 NewDifficulty)
{
	Difficulty = NewDifficulty;
}

int32 ABaseGameMode::GetDifficulty()
{
	return Difficulty;
}

void ABaseGameMode::ActorDied(AActor* DeadActor)
{
	return;
}

void ABaseGameMode::EnemyWon()
{
}

int32 ABaseGameMode::GetPreviousScore()
{
	return int32();
}

void ABaseGameMode::IncreaseAINumber()
{
	return;
}

float ABaseGameMode::GetBaseTurnRate()
{
	return BaseTurnRate;
}

void ABaseGameMode::PauseGame()
{
	return;
}

void ABaseGameMode::BeginPlay()
{
	Super::BeginPlay();
}


