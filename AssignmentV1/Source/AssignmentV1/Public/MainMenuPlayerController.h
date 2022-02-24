// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainMenuPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENTV1_API AMainMenuPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	// NN Display main menu
	void DisplayMenu();

protected:

	virtual void BeginPlay() override;

	// NN Main menu widget
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> MainMenuHUDClass;
	UPROPERTY()
	UUserWidget* MainMenuWidget;
	
};
