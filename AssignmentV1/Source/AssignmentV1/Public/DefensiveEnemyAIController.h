// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DefensiveEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENTV1_API ADefensiveEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:

	ADefensiveEnemyAIController();
	// NN Call AI's fire method
	void InvokeShootPlayer();

protected:
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
private:
	APawn* PlayerPawn;
	class AAICharacter* AIPawnRef;

	// NN How long to wait before firing to player again
	UPROPERTY(EditAnywhere)
	float AIFireRate = 1.f;

	// AI Behaviour tree
	UPROPERTY(EditAnywhere)
	UBehaviorTree* EnemyBehaviorTree;

	FTimerHandle KillAITimerHandle;
	FTimerHandle WaitTimerHandle;

	UPROPERTY(EditAnywhere)
	float KillAiTimer = 1.0f;

	void KillAI();

	void WaitTimer();
};
