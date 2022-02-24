// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Volume.h"
#include "Components/ArrowComponent.h"
#include "AISpawner.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENTV1_API AAISpawner : public AVolume
{
	GENERATED_BODY()
	
public:
	AAISpawner();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> AICharacter;

private:
	UArrowComponent* ForwardDirection;

	FTimerHandle DelaySpawnerHandle;

	UPROPERTY(EditAnywhere)
	float SpawnTimer;

	void SpawnAI();
};
