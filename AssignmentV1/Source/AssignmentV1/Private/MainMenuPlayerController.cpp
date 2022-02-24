// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuPlayerController.h"
#include "Blueprint/UserWidget.h"

void AMainMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();	
}

void AMainMenuPlayerController::DisplayMenu()
{
	MainMenuWidget = CreateWidget(this, MainMenuHUDClass);

	if (MainMenuWidget)
	{
		MainMenuWidget->AddToViewport();
		bShowMouseCursor = true;
		SetPause(true);
	}
}