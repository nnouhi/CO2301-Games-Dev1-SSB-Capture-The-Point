// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoPack.h"
#include "PlayerCharacter.h"
#include "Kismet/Gameplaystatics.h"
// Sets default values
AAmmoPack::AAmmoPack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Root->SetupAttachment(RootComponent);

	AmmoPackMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ammo pack mesh"));
	AmmoPackMesh->SetupAttachment(Root);
}

void AAmmoPack::KillAmmoPack()
{
	Cast<APlayerCharacter>(PlayerPawn)->SetbLineTrace(false);
	Destroy();
}

// Called when the game starts or when spawned
void AAmmoPack::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	
}

// Called every frame
void AAmmoPack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// NN Rotate medkit 1 unit per frame
	FQuat AmmoPackRotation = FQuat(FRotator(0.f, YawValue, 0.f));
	AddActorLocalRotation(AmmoPackRotation);
	

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
	if (PlayerPawn && FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation()) < ActivateLineTraceDistance)
	{
		Cast<APlayerCharacter>(PlayerPawn)->SetbLineTrace(true);
	}
}

