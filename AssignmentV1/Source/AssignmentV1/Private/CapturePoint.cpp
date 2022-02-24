// Fill out your copyright notice in the Description page of Project Settings.


#include "CapturePoint.h"
#include "PlayerCharacter.h"
#include "AICharacter.h"
#include "BaseGameMode.h"
#include "Kismet/Gameplaystatics.h"
#include "Components/BoxComponent.h"

// Sets default values
ACapturePoint::ACapturePoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	CollisionBox->SetupAttachment(Root);
	CollisionBox->SetCollisionProfileName("Trigger");

	FlagMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Flag Mesh"));
	FlagMesh->SetupAttachment(Root);

}


// Called when the game starts or when spawned
void ACapturePoint::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACapturePoint::OnOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ACapturePoint::OnOverlapEnd);
	
	StartingLocation = FlagMesh->GetRelativeLocation();

	// NN To calculate how far is the flag from being captured in %

	StartingFlagLocation = FlagMesh->GetRelativeLocation().Z;
	EndingFlagLocation = 2000.f; //hardcoded this is the end
	CurrentFlagLocation = 0;

	// NN Get game mode referece
	GameModeReference = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

}

// Called every frame
void ACapturePoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// NN If capture point is player's
	if (ActorHasTag("PlayersFlag"))
	{
		// NN If player isn't defending his point and there are other characters (AI) inside
		// Move the flag down and calculate how far down it went in %
		if (bPlayerDefendsHisFlag == false)
		{
			// NN Move flag down (speed increases by the amount of AI's their inside the capture point)
			NewLocation = FlagMesh->GetRelativeLocation();

			CurrentFlagLocation = FMath::Abs(NewLocation.Z);
	
			NewLocation.Z += (FlagSpeed * 2.f) * DeltaTime * NumOfCharactersInArea; // either 0 or num of rushing ai
			FlagMesh->SetRelativeLocation(NewLocation);	
		}

		// NN Percentage calculation
		float p, RemovedFractionalPart;

		// NN For example: ((2000+500)/2000) = 1.25 -> after mod RemovedFractionPart becomes 0.25f
		RemovedFractionalPart = FMath::Modf(((EndingFlagLocation + CurrentFlagLocation) / EndingFlagLocation), &p);
		// NN 0.25 * 100 = 25.0;
		RemovedFractionalPart *= 100;

		// Convert to int 25
		PlayersCapturePercentange = FMath::CeilToInt(RemovedFractionalPart);

	}
	
	// NN If capture point is AI's
	if (ActorHasTag("AIsFlag"))
	{
		// NN If AI aren't defending their point and thhe player character is inside
		// Move the flag down and calculate how far down it went in %
		if (bAIDefendsHisFlag == false)
		{
			NewLocation = FlagMesh->GetRelativeLocation();
			
			CurrentFlagLocation = FMath::Abs(NewLocation.Z);

			NewLocation.Z += FlagSpeed * DeltaTime * NumOfCharactersInArea; // either 0 or 1

			FlagMesh->SetRelativeLocation(NewLocation);
		}

		float p, RemovedFractionalPart;

		RemovedFractionalPart = FMath::Modf(((EndingFlagLocation + CurrentFlagLocation) / EndingFlagLocation), &p);
		RemovedFractionalPart *= 100;

		AIsCapturePercentange = FMath::CeilToInt(RemovedFractionalPart);
	}

	// NN Decide who won
	if (AIsCapturePercentange >= 100) //player won
	{
		if (GameModeReference)
		{
			GameModeReference->OnLevelComplete();
		}
	}
	else if (PlayersCapturePercentange >= 100) //ai won
	{
		if (GameModeReference)
		{
			GameModeReference->EnemyWon();
		}
	}
}

void ACapturePoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,	AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{

	// NN If capture point is player's
	if (ActorHasTag("PlayersFlag"))
	{
		// NN If player stepped inside his point activate defending mode (flag doesnt move until player dies or steps away)
		if (Cast<APlayerCharacter>(OtherActor))
		{
			bPlayerDefendsHisFlag = true;
		}
		// NN If ai stepped inside players point start moving the flag or increase speed 
		else if (Cast<AAICharacter>(OtherActor))
		{
			NumOfCharactersInArea++;
		}
	}

	// NN If capture point is AI's
	if (ActorHasTag("AISFlag"))
	{
		// NN If player is inside AI's point take the flag down
		if (Cast<APlayerCharacter>(OtherActor))
		{
			NumOfCharactersInArea++;
		}
		// NN If ai is isnide their point activate defensive mode
		else if (Cast<AAICharacter>(OtherActor))
		{
			bAIDefendsHisFlag = true;
		}
	}
}

void ACapturePoint::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// NN If capture point is player's
	if (ActorHasTag("PlayersFlag"))
	{
		// NN Flag can be captured
		if (Cast<APlayerCharacter>(OtherActor))
		{
			bPlayerDefendsHisFlag = false;
		}
		// NN IF AI left the players capture zone
		else if (Cast<AAICharacter>(OtherActor))
		{
			NumOfCharactersInArea--; // Decrease capture speed or if=0 stop flag capture
		}
	}

	// NN If capture point is AI's
	if (ActorHasTag("AISFlag"))
	{
		if (Cast<APlayerCharacter>(OtherActor))
		{
			NumOfCharactersInArea--; // Stop capture
		}
		// NN Flag can be captured
		else if (Cast<AAICharacter>(OtherActor))
		{
			bAIDefendsHisFlag = false;
		}
	}	
}

