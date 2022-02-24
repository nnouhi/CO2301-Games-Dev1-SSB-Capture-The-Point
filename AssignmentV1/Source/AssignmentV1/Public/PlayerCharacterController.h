// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharacterController.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENTV1_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()

public:
	// NN When player is near medkit or ammo pack make widget visible
	void HideInteractableWidget();

	// NN When player is not near medkit or ammo pack make widget invisible
	void DisplayInteractableWidget();

	// NN Display this widget once the player dies
	void GameOverDeath();

	// NN Display this widget once the player loses his capture point
	void GameOverLostCapture();

	// NN Display this widget once the player runs out of time 
	void GameOverTimesUp();

	// NN Display level clear widget
	void GameOverLevelClear();

	// NN Display this widget when player finishes both levels
	void GameOverGameClear();

	void DisplayPauseWidget();

protected:

	virtual void BeginPlay() override;
	
private:

	bool bExecuteOnce = false;

	// NN Crosshair widget
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> CrosshairHUDClass;
	UPROPERTY()
	UUserWidget* CrosshairWidget;

	// NN Healthbar widget
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> HealthbarHUDClass;
	UPROPERTY()
	UUserWidget* HealthbarWidget;

	// NN Ammunition widget
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> AmmunitionHUDClass;
	UPROPERTY()
	UUserWidget* AmmunitionWidget;

	// NN Remaining Enemies / Total Enemies widget
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> RemainingEnemiesHUDClass;
	UPROPERTY()
	UUserWidget* RemainingEnemiesWidget;

	// NN Level time remaining widget
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> RemainingLevelTimeHUDClass;
	UPROPERTY()
	UUserWidget* RemainingLevelTimeWidget;

	// NN Show percentage of how far players & ai's flags are from beinh captured widget
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> CapturedFlagsHUDClass;
	UPROPERTY()
	UUserWidget* CapturedFlagsWidget;

	// NN Widget when player is interacting with interactable actors
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> InteractionHUDClass;
	UPROPERTY()
	UUserWidget* InterectionWidget;

	// NN Widget once the player dies
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> GameOverDeathHUDClass;
	UPROPERTY()
	UUserWidget* GameOverDeathWidget;

	// NN Widget when player runs out of time
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> GameOverTimesUpHUDClass;
	UPROPERTY()
	UUserWidget* GameOverTimesUpWidget;

	// NN Widget when player finished level
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> LevelClearHUDClass;
	UPROPERTY()
	UUserWidget* LevelClearWidget;

	// NN Widget when player looses to AI
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> EnemyWonHUDClass;
	UPROPERTY()
	UUserWidget* EnemyWonWidget;

	// NN Widget when player finished both levels
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> GameClearHUDCLass;
	UPROPERTY()
	UUserWidget* GameClearWidget;

	// NN Widget to display minimap
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> MinimapHUDClass;
	UPROPERTY()
	UUserWidget* MinimapWidget;

	// NN Pause game widget
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> PauseHUDClass;
	UPROPERTY()
	UUserWidget* PauseWidget;
};
