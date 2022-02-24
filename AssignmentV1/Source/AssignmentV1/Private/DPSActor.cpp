// Fill out your copyright notice in the Description page of Project Settings.


#include "DPSActor.h"
#include "PlayerCharacter.h"
#include "AICharacter.h"
#include "Particles/ParticleSystemComponent.h"
#include "BaseGameMode.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADPSActor::ADPSActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	SphereComponent->SetupAttachment(Root);
	SphereComponent->SetCollisionProfileName("Trigger");

	Particles = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystem");
	Particles->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void ADPSActor::BeginPlay()
{
	Super::BeginPlay();

	// NN Lifetime of dps actor
	GetWorldTimerManager().SetTimer(
		DestroyTimerHandle,
		this,
		&ADPSActor::DestroyDPSActor,
		1.f,
		false
	);

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ADPSActor::OnOverlapBegin);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ADPSActor::OnOverlapEnd);

	
}

// Called every frame
void ADPSActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DamagedCharacters.Num() != 0)
	{
		for (int32 i = 0; i < DamagedCharacters.Num(); i++)
		{
			if (DamagedCharacters[i] != GetOwner())
			{
				DamageActor(DeltaTime, DamagedCharacters[i]);
			}
		}
	}	
}


void ADPSActor::DestroyDPSActor()
{
	Destroy();
}

void ADPSActor::DamageActor(float DeltaTime, AActor* ActorToDamage)
{
	// NN Apply Damage
	AController* MyOwnerInstigator = GetInstigatorController();
	UClass* DamageTypeClass = UDamageType::StaticClass();
	if (ActorToDamage)
	{
		UGameplayStatics::ApplyDamage(ActorToDamage, DamagePerSecond * DeltaTime, MyOwnerInstigator, this, DamageTypeClass);
	}
}

void ADPSActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (Cast<APlayerCharacter>(OtherActor) || Cast<AAICharacter>(OtherActor))
	{
		DamagedCharacters.Add(OtherActor);
	}
	
}

void ADPSActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		DamagedCharacters.Remove(OtherActor);
	}
}

