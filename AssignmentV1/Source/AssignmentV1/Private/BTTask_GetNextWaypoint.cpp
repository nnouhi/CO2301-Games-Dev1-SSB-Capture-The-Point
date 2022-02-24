// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetNextWaypoint.h"
#include "AIController.h"
#include "EnemyAIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

EBTNodeResult::Type UBTTask_GetNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	

	if (OwnerComp.GetAIOwner()==nullptr)
	{
		return EBTNodeResult::Failed;
		
	}
	
	AEnemyAIController* EnemyAIControllerReference = Cast<AEnemyAIController>(OwnerComp.GetOwner());

	FVector WaypointLocation = EnemyAIControllerReference->GetClosestWaypointLocation();
	// NN If reached last rushing waypoint clear the value 
	if (WaypointLocation == FVector(0, 0, 0))
	{
		OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("CaptureWaypoint"), FVector(0.f,0.f,0.f)); //works
		return EBTNodeResult::Succeeded;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), WaypointLocation);

	return EBTNodeResult::Succeeded;

	

}
