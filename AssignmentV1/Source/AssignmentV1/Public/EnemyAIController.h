// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENTV1_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:

	AEnemyAIController();
	void InvokeShootPlayer();
	FVector GetClosestWaypointLocation();
	FVector GetCaptureWaypoint();


protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltatTime) override;

	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

private:

	void KillAI();
	void WaitTimer();
	FTimerHandle WaitTimerHandle;
	// NN References
	APawn* AIPawn;
	APawn* PlayerPawn;
	class AAICharacter* AIPawnRef;

	// NN How long to wait before firing to player again
	UPROPERTY(EditAnywhere)
	float AIFireRate = 1.f;

	UPROPERTY(EditAnywhere)
	float KillAiTimer = 1.0f;

	// AI Behaviour tree
	UPROPERTY(EditAnywhere)
	UBehaviorTree* EnemyBehaviorTree;

	FTimerHandle ShooterTimerHandle;
	FTimerHandle KillAITimerHandle;

	TArray<AActor*> RushingWaypoints;
	TArray<AActor*> CaptureWaypoints;
	TArray<AActor*> InFrontOfPlayerWaypoints;
	int32 RandomIndex;

	AActor* CurrentWaypoint = nullptr;

};
