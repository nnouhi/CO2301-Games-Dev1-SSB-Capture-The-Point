// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelOneGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "MedKit.h"
#include "AmmoPack.h"
#include "Engine/TargetPoint.h"
#include "PlayerCharacterController.h"
#include "CapturePoint.h"
#include "AICharacter.h"

ALevelOneGameMode::ALevelOneGameMode()
{
	
}

void ALevelOneGameMode::ProgressNextLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Level2"));
}

FString ALevelOneGameMode::GetObjectiveMessage()
{
	return FString();
}

FString ALevelOneGameMode::GetLevelName()
{
	return TEXT("Level One");
}

int32 ALevelOneGameMode::GetScore()
{
	Score = (FMath::Abs(RemainingAI - TotalAI)) * TimeRemaining * GetDifficulty();
	PreviousLevelScore = Score;
	return Score;
}

void ALevelOneGameMode::ActorDied(AActor* DeadActor)
{
	// NN If an AI died
	if (Cast<AAICharacter>(DeadActor))
	{
		RemainingAI--;
		if (RemainingAI == 0)
		{
			OnLevelComplete();
		}

	}
	// NN If player character died
	else if (Cast<APlayerCharacter>(DeadActor))
	{
		// NN Destroy enemies and show died screen
		if (PlayerControllerReference)
		{
			PlayerControllerReference->GameOverDeath();
		}

		// NN Clear scene afte 2 seconds 
		ClearScene();
		
	}
}

FString ALevelOneGameMode::GetPlayersFlagPercentage()
{
	// NN Return the percentage of players flag from being captured to display on widget
	return FString::FromInt(Cast<ACapturePoint>(PlayersFlag[0])->GetPlayersCapturePercentage()) + TEXT(" %");
}

FString ALevelOneGameMode::GetAIsFlagPercentage()
{
	
	return FString::FromInt(Cast<ACapturePoint>(AIsFlag[0])->GetAIsCapturePercentange()) + TEXT(" %");

}

int32 ALevelOneGameMode::GetRemainingEnemies()
{
	return RemainingAI;
}

int32 ALevelOneGameMode::GetTotalEnemies()
{
	return TotalAI;
}

void ALevelOneGameMode::StartTimer()
{
	
	GetWorldTimerManager().SetTimer(
		LevelStartTimerHandle,
		this,
		&ALevelOneGameMode::TimesUp,
		LevelTimer,
		false
	);
}

void ALevelOneGameMode::TimesUp()
{
	if (PlayerControllerReference)
	{
		PlayerControllerReference->GameOverTimesUp();
	}
	ClearScene();

}

void ALevelOneGameMode::EnemyWon()
{
	if (PlayerControllerReference)
	{
		PlayerControllerReference->GameOverLostCapture();
	}
	ClearScene();

}

void ALevelOneGameMode::OnLevelComplete()
{
	if (PlayerControllerReference)
	{
		PlayerControllerReference->GameOverLevelClear();
	}
	ClearScene();

}

FString ALevelOneGameMode::GetTimeRemaining()
{
	// NN Get remaining time to convert to minutes/seconds
	TimeRemaining = GetWorldTimerManager().GetTimerRemaining(LevelStartTimerHandle);

	int Seconds = (FMath::FloorToInt(TimeRemaining) % 60);
	int Minutes = (FMath::FloorToInt(TimeRemaining) % 3600) / 60;

	return TEXT("Time Remain: ")
		+ FString::FromInt(Minutes) 
		+ TEXT(" m ") + FString::FromInt(Seconds) 
		+ TEXT(" s ");

}

void ALevelOneGameMode::IncreaseAINumber()
{
	TotalAI++;
	RemainingAI++;
}

void ALevelOneGameMode::PauseGame()
{
	if (PlayerControllerReference)
	{
		PlayerControllerReference->DisplayPauseWidget();
	}
}

void ALevelOneGameMode::BeginPlay()
{
	// NN Find out how many AIs are in the level
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAICharacter::StaticClass(), AIsInLevel);
	RemainingAI = AIsInLevel.Num();
	TotalAI = RemainingAI;

	LevelTimer = 240.f;

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("PlayersFlag"), PlayersFlag);
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("AIsFlag"), AIsFlag);

	PlayerControllerReference = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(this, 0));

	AdjustLevelBasedOnDifficulty();

	StartTimer();

	// NN Generate random waypoints 
	for (int32 i = 0; i < 100; i++)
	{
		float Theta = FMath::RandRange(0.f, 360.f); //Random angle
		float Radius = FMath::RandRange(0.f, 10000.f); //Random radius

		float x = Radius * FMath::Cos(Theta);
		float y = Radius * FMath::Sin(Theta);
		/*UE_LOG(LogTemp, Warning, TEXT("x: %f , y: %f"), x, y);*/

		ATargetPoint* TargetPoint = GetWorld()->SpawnActor<ATargetPoint>(ATargetPoint::StaticClass(), FVector(x, y, 10.f), FRotator(0.f, 0.f, 0.f));
		TargetPoint->Tags.Add(FName("RushingWaypoints")); //add tag for ai to know which waypoints those are
	}
	
	//// NN Play objective sound when game starts
	//UGameplayStatics::PlaySound2D(GetWorld(), ObjectiveSound);
}

void ALevelOneGameMode::AdjustLevelBasedOnDifficulty()
{
	// NN Set up all the variables that need to change based on choosen difficulty
	TArray<AActor*> Medkits;
	TArray<AActor*> AmmoPacks;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMedKit::StaticClass(), Medkits);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAmmoPack::StaticClass(), AmmoPacks);

	for (AActor* Medkit : Medkits)
	{
		Cast<AMedKit>(Medkit)->SetHPToRestore(GetDifficulty());
	}

	for (AActor* AmmoPack : AmmoPacks)
	{
		Cast<AAmmoPack>(AmmoPack)->SetAmmoToGive(GetDifficulty());
	}

	LevelTimer /= GetDifficulty();
}

void ALevelOneGameMode::ClearScene()
{
	TArray<AActor*> AI;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAICharacter::StaticClass(), AI);
	for (AActor* AIChar : AI)
	{
		AIChar->Destroy();
	}
}


