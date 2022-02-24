// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterController.h"
#include "Blueprint/UserWidget.h"


void APlayerCharacterController::BeginPlay()
{
	Super::BeginPlay();

	// NN Widget creation
	CrosshairWidget = CreateWidget(this, CrosshairHUDClass);
	HealthbarWidget = CreateWidget(this, HealthbarHUDClass);
	AmmunitionWidget = CreateWidget(this, AmmunitionHUDClass);
	RemainingEnemiesWidget = CreateWidget(this, RemainingEnemiesHUDClass);
	RemainingLevelTimeWidget = CreateWidget(this, RemainingLevelTimeHUDClass);
	CapturedFlagsWidget = CreateWidget(this, CapturedFlagsHUDClass);
	InterectionWidget = CreateWidget(this, InteractionHUDClass);
	GameOverDeathWidget = CreateWidget(this, GameOverDeathHUDClass);
	GameOverTimesUpWidget = CreateWidget(this, GameOverTimesUpHUDClass);
	LevelClearWidget = CreateWidget(this, LevelClearHUDClass);
	EnemyWonWidget = CreateWidget(this, EnemyWonHUDClass);
	GameClearWidget = CreateWidget(this, GameClearHUDCLass);
	MinimapWidget = CreateWidget(this, MinimapHUDClass);
	PauseWidget = CreateWidget(this, PauseHUDClass);

	if (CrosshairWidget)
	{
		CrosshairWidget->AddToViewport();
	}

	if (HealthbarWidget)
	{
		HealthbarWidget->AddToViewport();
	}

	if (AmmunitionWidget)
	{
		AmmunitionWidget->AddToViewport();
	}

	if (RemainingEnemiesWidget)
	{
		RemainingEnemiesWidget->AddToViewport();
	}
	
	if (RemainingLevelTimeWidget)
	{
		RemainingLevelTimeWidget->AddToViewport();
	}

	if (CapturedFlagsWidget)
	{
		CapturedFlagsWidget->AddToViewport();
	}

	if (MinimapWidget)
	{
		MinimapWidget->AddToViewport();
	}

	if (InterectionWidget)
	{
		InterectionWidget->AddToViewport();
		HideInteractableWidget(); // At first hide the widget
	}
}

void APlayerCharacterController::HideInteractableWidget()
{
	// Image's name is set in widget as "InteractionImage"
	InterectionWidget->GetWidgetFromName("InteractionImage")->SetVisibility(ESlateVisibility::Hidden);
}

void APlayerCharacterController::DisplayInteractableWidget()
{
	InterectionWidget->GetWidgetFromName("InteractionImage")->SetVisibility(ESlateVisibility::Visible);
}

void APlayerCharacterController::GameOverDeath()
{
	if (GameOverDeathWidget)
	{
		if (bExecuteOnce == false)
		{
			GameOverDeathWidget->AddToViewport();
			bExecuteOnce = true;
		}
	}
}

void APlayerCharacterController::GameOverLostCapture()
{
	if (EnemyWonWidget)
	{
		if (bExecuteOnce == false)
		{
			EnemyWonWidget->AddToViewport();
			bExecuteOnce = true;
		}
	}
}

void APlayerCharacterController::GameOverTimesUp()
{
	if (GameOverTimesUpWidget)
	{
		if (bExecuteOnce == false)
		{
			GameOverTimesUpWidget->AddToViewport();
			bExecuteOnce = true;
		}
	}
}

void APlayerCharacterController::GameOverLevelClear()
{
	if (LevelClearWidget)
	{
		if (bExecuteOnce == false)
		{
			LevelClearWidget->AddToViewport();
			bExecuteOnce = true;
		}
	}
}

void APlayerCharacterController::GameOverGameClear()
{
	if (GameClearWidget)
	{
		if (bExecuteOnce == false)
		{
			GameClearWidget->AddToViewport();
			bExecuteOnce = true;
		}
	}
}

void APlayerCharacterController::DisplayPauseWidget()
{
	if (PauseWidget)
	{
		PauseWidget->AddToViewport();
	}
}

