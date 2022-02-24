// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DPSActor.generated.h"

UCLASS()
class ASSIGNMENTV1_API ADPSActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADPSActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	// NN Root comp
	UPROPERTY(EditAnywhere)
	USceneComponent* Root;

	// NN Particles to be played when spawned
	UPROPERTY(EditAnywhere)
	class UParticleSystemComponent* Particles;

	UPROPERTY(EditAnywhere)
	class USphereComponent* SphereComponent;

	// NN If player/ai is inside the damage distance
	UPROPERTY(EditAnywhere)
	float DamageDistance = 500.f;

	// NN DPS
	UPROPERTY(EditAnywhere)
	float DamagePerSecond = 10.f;

	float Distance = 0.f;
	bool bApplyDamage = false;
	TArray<AActor*> DamagedCharacters;
	FTimerHandle DestroyTimerHandle;
	FTimerHandle CloseCharactersTimerHandle;

	void DestroyDPSActor();

	void DamageActor(float DeltaTime, AActor* ActorToDamage);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		 const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
