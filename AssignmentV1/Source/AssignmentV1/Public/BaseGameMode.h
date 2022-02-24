// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENTV1_API ABaseGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	// NN Base class methods
	UFUNCTION(BlueprintCallable)
	virtual void ProgressNextLevel();

	UFUNCTION(BlueprintCallable)
	virtual FString GetObjectiveMessage();
	
	UFUNCTION(BlueprintCallable)
	virtual FString GetLevelName();

	UFUNCTION(BlueprintCallable)
	virtual void OnLevelComplete();

	UFUNCTION(BlueprintCallable)
	virtual int32 GetScore();

	UFUNCTION(BlueprintCallable)
	virtual int32 GetRemainingEnemies();

	UFUNCTION(BlueprintCallable)
	virtual int32 GetTotalEnemies();

	UFUNCTION(BlueprintCallable)
	virtual FString GetTimeRemaining();

	UFUNCTION(BlueprintCallable)
	virtual FString GetPlayersFlagPercentage();

	UFUNCTION(BlueprintCallable)
	virtual FString GetAIsFlagPercentage();

	UFUNCTION(BlueprintCallable)
	virtual void SetDifficulty(int32 NewDifficulty);

	UFUNCTION(BlueprintCallable)
	virtual int32 GetDifficulty();

	virtual void ActorDied(AActor* DeadActor);

	virtual void EnemyWon();

	UFUNCTION(BlueprintCallable)
	virtual int32 GetPreviousScore();

	virtual void IncreaseAINumber();

	UFUNCTION(BlueprintCallable)
	virtual float GetBaseTurnRate();

	UFUNCTION(BlueprintCallable)
	virtual void SetBaseTurnRate(float SetTurnRate) { BaseTurnRate = SetTurnRate; };

	UFUNCTION()
	virtual void PauseGame();

	
protected:

	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	float LevelTimer=120.f;

	int32 RemainingAI=0;
	int32 TotalAI = 0;
	int32 Score = 0;
	static int32 PreviousLevelScore;
	int32 AllLevelsScore = 0;

	

private:

	static int32 Difficulty;
	static float BaseTurnRate;

	

};
