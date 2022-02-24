// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseGameMode.h"
#include "LevelOneGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENTV1_API ALevelOneGameMode : public ABaseGameMode
{
	GENERATED_BODY()

public:

	ALevelOneGameMode();

	// NN Player progresses to next level
	void ProgressNextLevel() override;

	// NN Whats the objective of the level
	FString GetObjectiveMessage() override;

	// NN Whats the name of the level
	FString GetLevelName() override;

	// NN Logic to do at the completion of the level
	void OnLevelComplete() override;

	// NN Calculate players score based on timer left and enemies killed
	int32 GetScore() override;

	// NN Based on which actor died implement logic
	void ActorDied(AActor* DeadActor) override;

	// NN Get remaining enemies
	int32 GetRemainingEnemies() override;

	// NN Get total enemies
	int32 GetTotalEnemies() override;

	// NN Invoked once the level starts
	void StartTimer();

	// NOTE: DELETE LATER ITS JUST FOR TESTING (IDEALY CALL PLAYER CONTROLLER TO DISPLAY 'YOU LOST' WIDGET)
	void TimesUp();

	void EnemyWon() override;

	// NN Get the remaining level time
	FString GetTimeRemaining() override;

	// NN Get the remaining % of players flag from being captured
	FString GetPlayersFlagPercentage() override;

	// NN Get the remaining % of AIs flag from being captured
	FString GetAIsFlagPercentage() override;

	class ACapturePoint* CapturePointRef;

	// NN This method will be called to increase the number of total AI and ai that are in level
	void IncreaseAINumber() override;

	// NN Invoked when player pauses the game
	void PauseGame() override;

	

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
	class USoundBase* ObjectiveSound;

	// NN Timer handle for level time
	FTimerHandle LevelStartTimerHandle;

	TArray<AActor*> PlayersFlag;
	TArray<AActor*> AIsFlag;
	TArray<AActor*> AIsInLevel;

	class APlayerCharacterController* PlayerControllerReference;

	// NN Adjust the level based on the difficulty set
	void AdjustLevelBasedOnDifficulty();

	float TimeRemaining = 0.f;

	// NN On end destroy all AI
	void ClearScene();

};
