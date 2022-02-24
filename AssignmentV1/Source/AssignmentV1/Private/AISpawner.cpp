// Fill out your copyright notice in the Description page of Project Settings.


#include "AISpawner.h"
#include "NavigationSystem.h"
#include "AICharacter.h"
#include "BaseGameMode.h"
#include "Kismet/Gameplaystatics.h"


AAISpawner::AAISpawner()
{
	ForwardDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow Component"));
}

void AAISpawner::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(
		DelaySpawnerHandle,
		this,
		&AAISpawner::SpawnAI,
		SpawnTimer,
		true
	);
}

void AAISpawner::SpawnAI()
{
	ABaseGameMode* GameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	// Increase ai counter 
	if (GameMode)
	{
		GameMode->IncreaseAINumber();
	}

	FNavLocation SpawnLocation;

	UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	if (NavigationSystem->GetRandomPointInNavigableRadius(GetBounds().Origin, GetBounds().BoxExtent.Size(), SpawnLocation))
	{
		FRotator SpawnRotation = GetActorRotation();
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		GetWorld()->SpawnActor<AActor>(AICharacter, SpawnLocation.Location, SpawnRotation, SpawnParams);
	}
}
