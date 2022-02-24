// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExplosiveBarrel.generated.h"

UCLASS()
class ASSIGNMENTV1_API AExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExplosiveBarrel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// NN When invoke barrel explodes
	void ExplodeBarrel();
	
	UFUNCTION()
	void Damage(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BarrelMeshComponent;

	UPROPERTY(EditAnywhere)
	class URadialForceComponent* ForceComponent;

	UPROPERTY(EditAnywhere)
	class UDestructibleComponent* DestructibleMesh;

	TArray<AActor*> Characters;

	// NN Particle of the projectile when it lands
	UPROPERTY(EditAnywhere)
	class UParticleSystem* ExplosionParticle;
	// NN Damage to apply to destructible mesh
	UPROPERTY(EditAnywhere)
	float DamageAmount = 10.f;

	// NN Impulse strength to apply
	UPROPERTY(EditAnywhere)
	float ImpulseStrength = 100000.f;

	// NN In time of explosion if anyone is in this distance deal damage
	UPROPERTY(EditAnywhere)
	float DamageDistance = 200.f;

	// NN Damage to deal to anyone in < DamageDistance
	UPROPERTY(EditAnywhere)
	float DamageDealt = 30.f;

	// NN Sound to play
	UPROPERTY(EditAnywhere)
	class USoundBase* ExplosionSound;
};
