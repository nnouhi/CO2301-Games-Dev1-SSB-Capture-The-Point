// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TargetPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AICharacter.h"

AEnemyAIController::AEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true; //true for now
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	
	PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);

	GetWorldTimerManager().SetTimer(
		WaitTimerHandle,
		this,
		&AEnemyAIController::WaitTimer,
		1.f,
		false
	);
	
	
}

void AEnemyAIController::InvokeShootPlayer()
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

FVector AEnemyAIController::GetClosestWaypointLocation()
{
	if (AIPawnRef)
	{
		if (!AIPawnRef->IsDead())
		{
			// NN Check if player navigated all the waypoints
			if (InFrontOfPlayerWaypoints.Num() != 0)
			{
				// NN Delete previous navigated waypoint so it doesnt move to it again (stay stationary)
				if (CurrentWaypoint != nullptr)
				{
					for (int32 i = 0; i < InFrontOfPlayerWaypoints.Num(); i++)
					{
						if (CurrentWaypoint == InFrontOfPlayerWaypoints[i])
						{
							InFrontOfPlayerWaypoints.RemoveAt(i);
							InFrontOfPlayerWaypoints.Shrink();
							CurrentWaypoint = nullptr;
							break;
						}
					}
					// NN Previous navigated waypoint was the last in the array and it was removed, return
					if (InFrontOfPlayerWaypoints.Num() == 0)
					{
						return FVector(0, 0, 0);
					}
				}

				AActor* TempWaypoint = nullptr;
				RandomIndex = FMath::RandRange(0, InFrontOfPlayerWaypoints.Num() - 1); // NN Get random waypoint thats infront of AI

				TempWaypoint = InFrontOfPlayerWaypoints[RandomIndex]; // NN Waypoint thats selected

				// NN Itterate the array and find the waypoints that are previous to the waypoint selected and remove them from the array
				for (int32 i = 0; i < InFrontOfPlayerWaypoints.Num(); i++)
				{
					if (TempWaypoint->GetActorLocation().X <= InFrontOfPlayerWaypoints[i]->GetActorLocation().X)
					{
						InFrontOfPlayerWaypoints.RemoveAt(i); // remove the waypoint from the array just for efficiency
						InFrontOfPlayerWaypoints.Shrink(); // free memory
						i--;
					}
				}
				/*UE_LOG(LogTemp, Warning, TEXT("%i"), InFrontOfPlayerWaypoints.Num());*/
				CurrentWaypoint = TempWaypoint;
				return TempWaypoint->GetActorLocation();
			}

			return FVector(0, 0, 0);
		}
	}
	return FVector(0, 0, 0);

}

FVector AEnemyAIController::GetCaptureWaypoint()
{	
	if (AIPawnRef)
	{
		if (!AIPawnRef->IsDead())
		{
			// Return a random capture waypoint
			return CaptureWaypoints[(FMath::RandRange(0, CaptureWaypoints.Num() - 1))]->GetActorLocation();
		}
	}
	return FVector(0, 0, 0);

}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// NN Player death animation then detroy
	if (AIPawnRef)
	{
		if (AIPawnRef->IsDead())
		{
			GetWorldTimerManager().SetTimer(
				KillAITimerHandle,
				this,
				&AEnemyAIController::KillAI,
				KillAiTimer,
				false //loop
			);

			SetActorTickEnabled(false);
		}
	}
	
	/*UE_LOG(LogTemp, Warning, TEXT("hp: %f"), AIPawnRef->GetHealthPercentage());*/
}

void AEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{

}

void AEnemyAIController::KillAI()
{
	AIPawnRef->Destroy();
}

void AEnemyAIController::WaitTimer()
{
	// Cast
	AIPawn = GetPawn();
	AIPawnRef = Cast<AAICharacter>(AIPawn);


	// NN If set from editor run
	if (EnemyBehaviorTree)
	{
		RunBehaviorTree(EnemyBehaviorTree);
		GetBlackboardComponent()->SetValueAsVector(TEXT("NearestWaypoint"), FVector(0.f, 0.f, 0.f)); // set default value for nearest waypoint
	}

	// NN Populate the array with all the waypoints that will be used by the 'rushing AI'
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ATargetPoint::StaticClass(), TEXT("RushingWaypoints"), RushingWaypoints);

	// NN Populate the array with all the waypoints that are nearby the capture area
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ATargetPoint::StaticClass(), TEXT("CaptureWaypoint"), CaptureWaypoints);

	// NN If a waypoint is in front of the player add it to the array
	for (AActor* Waypoint : RushingWaypoints)
	{
		if (Waypoint->GetActorLocation().X < AIPawn->GetActorLocation().X)
		{
			InFrontOfPlayerWaypoints.Add(Waypoint);
		}
	}

}
