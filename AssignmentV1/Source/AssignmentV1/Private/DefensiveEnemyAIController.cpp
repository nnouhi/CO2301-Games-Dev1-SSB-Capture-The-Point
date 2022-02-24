// Fill out your copyright notice in the Description page of Project Settings.


#include "DefensiveEnemyAIController.h"
#include "AICharacter.h"
#include "Kismet/GameplayStatics.h"


ADefensiveEnemyAIController::ADefensiveEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true; //true for now
}

void ADefensiveEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(
		WaitTimerHandle,
		this,
		&ADefensiveEnemyAIController::WaitTimer,
		1.f,
		false
	);

}

void ADefensiveEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (AIPawnRef)
	{
		if (AIPawnRef->IsDead())
		{
			GetWorldTimerManager().SetTimer(
				KillAITimerHandle,
				this,
				&ADefensiveEnemyAIController::KillAI,
				KillAiTimer,
				false //loop
			);
			SetActorTickEnabled(false);
		}
	}
	
}

void ADefensiveEnemyAIController::KillAI()
{
	AIPawnRef->Destroy();
}

void ADefensiveEnemyAIController::WaitTimer()
{
	PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	AIPawnRef = Cast<AAICharacter>(GetPawn());
	if (EnemyBehaviorTree)
	{
		RunBehaviorTree(EnemyBehaviorTree);
	}
}

void ADefensiveEnemyAIController::InvokeShootPlayer()
{
	if (AIPawnRef)
	{
		if (!AIPawnRef->IsDead())
		{
			SetFocus(PlayerPawn);
			AIPawnRef->ShootPlayer();
		}
	}
	
}