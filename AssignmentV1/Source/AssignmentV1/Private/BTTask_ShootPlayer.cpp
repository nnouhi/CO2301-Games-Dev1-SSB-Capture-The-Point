// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ShootPlayer.h"
#include "AIController.h"
#include "EnemyAIController.h"
#include "DefensiveEnemyAIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

EBTNodeResult::Type UBTTask_ShootPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner())
	{
		if (OwnerComp.GetAIOwner()->IsA(ADefensiveEnemyAIController::StaticClass()))
		{
			ADefensiveEnemyAIController* DefensiveEnemyAIControllerReference = Cast<ADefensiveEnemyAIController>(OwnerComp.GetAIOwner());
			DefensiveEnemyAIControllerReference->InvokeShootPlayer();
			return EBTNodeResult::Succeeded;
		}
		else if (OwnerComp.GetAIOwner()->IsA(AEnemyAIController::StaticClass()))
		{
			AEnemyAIController* EnemyAIControllerReference = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
			EnemyAIControllerReference->InvokeShootPlayer();
			return EBTNodeResult::Succeeded;
		}
		
	}

	return EBTNodeResult::Failed;
}