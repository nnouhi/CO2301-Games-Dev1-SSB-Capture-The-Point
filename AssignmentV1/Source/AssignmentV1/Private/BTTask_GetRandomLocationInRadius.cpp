// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetRandomLocationInRadius.h"
#include "AIController.h"
#include "AICharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_GetRandomLocationInRadius::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() == nullptr )
	{
		return EBTNodeResult::Failed;
	}

	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if(!Cast<AAICharacter>(AIPawn)->IsDead())
	{
		// NOTE: Increase or Decrease when testing
		float NavigableRadius = 500.f; // NN In range of...
		FNavLocation RandomNavigableLocation; // NN Will hold the location that the AI will navigate to

		// NN Obtain nav system reference
		UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);

		NavSys->GetRandomReachablePointInRadius(AIPawn->GetActorLocation(), NavigableRadius, RandomNavigableLocation);

		OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), RandomNavigableLocation);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}