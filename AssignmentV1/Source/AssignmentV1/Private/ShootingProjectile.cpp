// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "DPSActor.h"
#include "ExplosiveBarrel.h"
#include "PlayerCharacter.h"
#include "AICharacter.h"

// Sets default values
AShootingProjectile::AShootingProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    // NN Set up components and their attributes
    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
    RootComponent = ProjectileMesh;

    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));

    ProjectileMovementComponent->InitialSpeed = 2000.f;
    ProjectileMovementComponent->MaxSpeed = 2000.f;

    Particles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particles"));
    Particles->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AShootingProjectile::BeginPlay()
{
	Super::BeginPlay();

    ProjectileMesh->OnComponentHit.AddDynamic(this, &AShootingProjectile::OnHit);

    // NN OnSpawn play sound
    if (LaunchSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
    }
}

// Called every frame
void AShootingProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AShootingProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    AActor* ProjectileOwner = GetOwner();
   
    // NN Don't do any logic
    if (ProjectileOwner == nullptr)
    {
        Destroy();
        return;
    }
    
    AController* MyOwnerInstigator = ProjectileOwner->GetInstigatorController();
    UClass* DamageTypeClass = UDamageType::StaticClass();

    // NN If other actor is valid && other actor not player && other actor is not the projectile
    // do logic
    if (OtherActor && OtherActor != ProjectileOwner && OtherActor != this)
    {
        // NN If hit a player character (Can be either AI Or Player character)
        if (Cast<AAICharacter>(OtherActor) || Cast<APlayerCharacter>(OtherActor))
        {
            // NN Apply damage to the health comp of ther other actor
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, GetActorLocation(), GetActorRotation(),true);
            UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwnerInstigator, this, DamageTypeClass);
        }
        else if (Cast<AExplosiveBarrel>(OtherActor))
        {
           /* Cast<AExplosiveBarrel>(OtherActor)->ExplodeBarrel();*/
        }
        // NN Spawn an orbit that deals damage per second 
        else
        {
            // NN Get DPS spawnpoint location and rotation
            FVector DPSActorLocation = GetActorLocation();
            FRotator DPSActorRotation = GetActorRotation();

            //AShootingProjectile* Projectile = GetWorld()->SpawnActor<AShootingProjectile>(ProjectileClass, ProjectileSpawnPointLocation, ProjectileSpawnPointRotation);
            // NN On hit with any other object except a character spawn the dps actor
            ADPSActor* DPSActor = GetWorld()->SpawnActor<ADPSActor>(DPSActorClass, DPSActorLocation, DPSActorRotation);
            
            // NN Set its owner the character that shot it to not do damage to him
            DPSActor->SetOwner(ProjectileOwner);
        
            // NN Play hit sound
            if (HitSound)
            {
                UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
            }
        }   
    }

     Destroy(); //end
}

