// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckLOS.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"


void UBTService_CheckLOS::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// NN Obtain references
	AAIController* AIControllerReference = OwnerComp.GetAIOwner();
	APawn* PlayerReference = UGameplayStatics::GetPlayerPawn(this, 0);
	APawn* AIPawn = AIControllerReference->GetPawn();



	if (AIPawn && PlayerReference)
	{
		// NN If LOS proceed to check if player is infront of AI
		if (AIControllerReference->LineOfSightTo(PlayerReference))
		{
			/*UE_LOG(LogTemp, Warning, TEXT("%s"), *AIPawn->GetName());*/

			// NN == DOT PRODUCT ==
			FVector AIPawnForwardVector = AIPawn->GetActorForwardVector(); //Forward vector is already normilized
			FVector PlayerLocation = PlayerReference->GetActorLocation();
			FVector AIPawnLocation = AIPawn->GetActorLocation();
			FVector AIToPlayerVector = PlayerLocation - AIPawnLocation;

			if (FVector::DotProduct(AIToPlayerVector, AIPawnForwardVector) > 0.f) //infront
			{

				OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), true);
			}
			else
			{
				OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
			}
		}
		else
		{
			OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
		}
		return;
	}

	return;
}