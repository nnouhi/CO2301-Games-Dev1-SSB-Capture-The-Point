// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"
#include "Kismet/Gameplaystatics.h"
#include "MainMenuPlayerController.h"

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	AMainMenuPlayerController* MainMenuPlayerControllerReference = Cast<AMainMenuPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	if (MainMenuPlayerControllerReference)
	{
		MainMenuPlayerControllerReference->DisplayMenu();
	}
}