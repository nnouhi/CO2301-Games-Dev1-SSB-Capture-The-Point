// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveBarrel.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "ShootingProjectile.h"
#include "DestructibleComponent.h"


// Sets default values
AExplosiveBarrel::AExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BarrelMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelMeshComponent"));
	RootComponent = BarrelMeshComponent;
	BarrelMeshComponent->SetSimulatePhysics(true);

	ForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("ForceComponent"));
	ForceComponent->SetupAttachment(BarrelMeshComponent);

	DestructibleMesh = CreateDefaultSubobject<UDestructibleComponent>(TEXT("DestructibleMesh"));
	DestructibleMesh->SetupAttachment(BarrelMeshComponent);

}

// Called when the game starts or when spawned
void AExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
	// NN When anything damages the barrel (excluding player colliding with it)
	DestructibleMesh->OnComponentHit.AddDynamic(this, &AExplosiveBarrel::Damage);
}

// Called every frame
void AExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExplosiveBarrel::ExplodeBarrel()
{
	
	
}

void AExplosiveBarrel::Damage(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->IsA(AShootingProjectile::StaticClass()))
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), Characters);
		
		DestructibleMesh->ApplyDamage(DamageAmount, DestructibleMesh->GetComponentLocation(), DestructibleMesh->GetForwardVector(), ImpulseStrength);
		
		ForceComponent->FireImpulse();

		for (AActor* Character : Characters)
		{
			if (FVector::Dist(GetActorLocation(), Character->GetActorLocation()) < DamageDistance)
			{
				AController* MyOwnerInstigator = GetInstigatorController();
				UClass* DamageTypeClass = UDamageType::StaticClass();

				UGameplayStatics::ApplyDamage(Character, DamageDealt, MyOwnerInstigator, this, DamageTypeClass);
			}
		}

		if (ExplosionSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
		}

		if (ExplosionParticle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, GetActorLocation(), GetActorRotation(),FVector(3.f,3.f,3.f), true);
		}
	}
	
}

