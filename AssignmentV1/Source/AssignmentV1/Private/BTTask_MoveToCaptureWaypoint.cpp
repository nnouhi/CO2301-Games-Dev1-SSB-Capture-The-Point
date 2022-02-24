// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveToCaptureWaypoint.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAIController.h"

EBTNodeResult::Type UBTTask_MoveToCaptureWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AEnemyAIController* EnemyAIControllerReference = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

	FVector WaypointLocation = EnemyAIControllerReference->GetCaptureWaypoint();

	if (WaypointLocation == FVector(0, 0, 0))
	{
		OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
		return EBTNodeResult::Succeeded;
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), WaypointLocation);
	return EBTNodeResult::Succeeded;

}