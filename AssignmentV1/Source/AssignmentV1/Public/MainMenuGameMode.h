// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseGameMode.h"
#include "MainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENTV1_API AMainMenuGameMode : public ABaseGameMode
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;
	
};
