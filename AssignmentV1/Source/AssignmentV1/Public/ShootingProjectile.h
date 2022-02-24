// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShootingProjectile.generated.h"

UCLASS()
class ASSIGNMENTV1_API AShootingProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShootingProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    // NN Projectile movement component to set InitialSpeed(Current speed base on acceleration and gravity)
    // and MaxSpeed (projectile's velocity threshhold) of projectile
    UPROPERTY(EditAnywhere)
    class UProjectileMovementComponent* ProjectileMovementComponent;

    FORCEINLINE void SetProjectileDamage(float Amount) { Damage = Amount; };

private:

    // NN Mesh that will do the collision checks
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* ProjectileMesh;

    // NN Particle of the projectile
    UPROPERTY(EditAnywhere)
    class UParticleSystemComponent* Particles;

    // NN Particle of the projectile when it lands
    UPROPERTY(EditAnywhere)
    class UParticleSystem* HitParticles;

    // NN Sound when player shoots
    UPROPERTY(EditAnywhere)
    USoundBase* LaunchSound;

    // NN Sound when the projectile lands
    UPROPERTY(EditAnywhere)
    USoundBase* HitSound;

    // NN How much damage does the projectile do (NOTE: Edit from editor for testing)
    UPROPERTY(EditAnywhere)
    float Damage = 10.f;

    // NN Method that handles logic on collision
    UFUNCTION()
    virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    // NN DPS Actor sub class
    UPROPERTY(EditAnywhere)
    TSubclassOf<class ADPSActor> DPSActorClass;

};
