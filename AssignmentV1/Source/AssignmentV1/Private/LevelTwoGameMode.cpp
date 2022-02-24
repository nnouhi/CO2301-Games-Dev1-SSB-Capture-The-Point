// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTwoGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "MedKit.h"
#include "AmmoPack.h"
#include "Engine/TargetPoint.h"
#include "PlayerCharacterController.h"
#include "CapturePoint.h"
#include "AICharacter.h"

ALevelTwoGameMode::ALevelTwoGameMode()
{

}

void ALevelTwoGameMode::ProgressNextLevel()
{
	// NOTE: Maybe add level 3?
	/*UGameplayStatics::OpenLevel(GetWorld(), TEXT("Level_1"));*/
}

FString ALevelTwoGameMode::GetObjectiveMessage()
{
	return FString();
}

FString ALevelTwoGameMode::GetLevelName()
{
	return TEXT("Level Two");
}

int32 ALevelTwoGameMode::GetScore()
{
	Score = (FMath::Abs(RemainingAI - TotalAI)) * TimeRemaining * GetDifficulty();
	return Score;
}

void ALevelTwoGameMode::ActorDied(AActor* DeadActor)
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
		
		if (PlayerControllerReference)
		{
			PlayerControllerReference->GameOverDeath();
		}
		ClearScene();
	}
}

FString ALevelTwoGameMode::GetPlayersFlagPercentage()
{

	// NN Return the percentage of players flag from being captured to display on widget
	return FString::FromInt(Cast<ACapturePoint>(PlayersFlag[0])->GetPlayersCapturePercentage()) + TEXT(" %");
}

FString ALevelTwoGameMode::GetAIsFlagPercentage()
{

	return FString::FromInt(Cast<ACapturePoint>(AIsFlag[0])->GetAIsCapturePercentange()) + TEXT(" %");

}

int32 ALevelTwoGameMode::GetPreviousScore()
{
	return PreviousLevelScore;
}

int32 ALevelTwoGameMode::GetRemainingEnemies()
{
	return RemainingAI;
}

int32 ALevelTwoGameMode::GetTotalEnemies()
{
	return TotalAI;
}

void ALevelTwoGameMode::StartTimer()
{

	GetWorldTimerManager().SetTimer(
		LevelStartTimerHandle,
		this,
		&ALevelTwoGameMode::TimesUp,
		LevelTimer,
		false
	);
}

void ALevelTwoGameMode::TimesUp()
{
	if (PlayerControllerReference)
	{
		PlayerControllerReference->GameOverTimesUp();
	}
	ClearScene();
}

void ALevelTwoGameMode::EnemyWon()
{
	if (PlayerControllerReference)
	{
		PlayerControllerReference->GameOverLostCapture();
	}
	ClearScene();
}

void ALevelTwoGameMode::OnLevelComplete()
{
	if (PlayerControllerReference)
	{
		PlayerControllerReference->GameOverGameClear();
	}
	ClearScene();
}

FString ALevelTwoGameMode::GetTimeRemaining()
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

void ALevelTwoGameMode::IncreaseAINumber()
{
	TotalAI++;
	RemainingAI++;
}

void ALevelTwoGameMode::PauseGame()
{
	if (PlayerControllerReference)
	{
		PlayerControllerReference->DisplayPauseWidget();
	}
}

void ALevelTwoGameMode::BeginPlay()
{
	// NN Find out how many AIs are in the level
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAICharacter::StaticClass(), AIsInLevel);
	RemainingAI = AIsInLevel.Num();
	TotalAI = RemainingAI;

	LevelTimer = 480.f;

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("PlayersFlag"), PlayersFlag);
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("AIsFlag"), AIsFlag);

	PlayerControllerReference = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(this, 0));

	AdjustLevelBasedOnDifficulty();

	StartTimer();

	// NN Generate random waypoints 
	for (int32 i = 0; i < 100; i++)
	{
		float Theta = FMath::RandRange(0.f, 360.f); //Random angle
		float Radius = FMath::RandRange(0.f, 3000.f); //Random radius

		float x = Radius * FMath::Cos(Theta);
		float y = Radius * FMath::Sin(Theta);
		/*UE_LOG(LogTemp, Warning, TEXT("x: %f , y: %f"), x, y);*/

		ATargetPoint* TargetPoint = GetWorld()->SpawnActor<ATargetPoint>(ATargetPoint::StaticClass(), FVector(x, y, 10.f), FRotator(0.f, 0.f, 0.f));
		TargetPoint->Tags.Add(FName("RushingWaypoints")); //add tag for ai to know which waypoints those are
	}
	//// NN Play objective sound when game starts
	//UGameplayStatics::PlaySound2D(GetWorld(), ObjectiveSound);
}

void ALevelTwoGameMode::AdjustLevelBasedOnDifficulty()
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

void ALevelTwoGameMode::ClearScene()
{
	// NN Destroy enemies and show died screen
	TArray<AActor*> AI;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAICharacter::StaticClass(), AI);
	for (AActor* AIChar : AI)
	{
		AIChar->Destroy();
	}
}




