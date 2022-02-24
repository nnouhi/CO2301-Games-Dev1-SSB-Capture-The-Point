// Fill out your copyright notice in the Description page of Project Settings.


#include "MedKit.h"
#include "PlayerCharacter.h"
#include "BaseGameMode.h"
#include "Kismet/Gameplaystatics.h"

// Sets default values
AMedKit::AMedKit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// NN Set up components
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene"));
	Root->SetupAttachment(RootComponent);

	MedkitMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Medkit Mesh"));
	MedkitMesh->SetupAttachment(Root);
}


// Called when the game starts or when spawned
void AMedKit::BeginPlay()
{
	Super::BeginPlay();
	PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	/*UE_LOG(LogTemp, Warning, TEXT("Hp to restore: %f"), HPToRestore);*/

}

// Called every frame
void AMedKit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// NN Rotate medkit 1 unit per frame
	FQuat MedkitRotation = FQuat(FRotator(0.f, YawValue, 0.f));
	AddActorLocalRotation(MedkitRotation);

	// NN Move up and down as a sine wave
	FVector Location = FVector::ZeroVector;
	Location.Z += FMath::Sin(Angle) * 0.5f;
	Angle += DeltaTime;

	if (Angle > 180)
	{
		Angle = 0.f;
	}
	AddActorLocalOffset(Location, true);


	// NN Calculate distance between med kit and player
	// if close activate player's line trace 
	if (PlayerPawn && FVector::Dist(GetActorLocation(),PlayerPawn->GetActorLocation()) < ActivateLineTraceDistance)
	{
		Cast<APlayerCharacter>(PlayerPawn)->SetbLineTrace(true);
	}
}

void AMedKit::KillMedkit()
{
	Cast<APlayerCharacter>(PlayerPawn)->SetbLineTrace(false);
	Destroy();
}


