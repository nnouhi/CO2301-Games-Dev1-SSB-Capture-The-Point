// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_DistancePlayerFromAI.h"
#include "AIController.h"
#include "Kismet/Gameplaystatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void UBTService_DistancePlayerFromAI::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);

	if (AIPawn)
	{
		float Distance = FVector::Dist(AIPawn->GetActorLocation(), PlayerPawn->GetActorLocation());
		/*UE_LOG(LogTemp, Warning, TEXT("%f"), Distance);*/
		if (Distance < 3000.f)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), true);
			return;
		}
		OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
		return;
	}
	
}
