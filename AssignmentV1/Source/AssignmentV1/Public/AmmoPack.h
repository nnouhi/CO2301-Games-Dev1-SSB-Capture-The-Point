// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmoPack.generated.h"

UCLASS()
class ASSIGNMENTV1_API AAmmoPack : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAmmoPack();

	FORCEINLINE void SetAmmoToGive(int32 Amount) { AmmoToGive /= Amount; };
	FORCEINLINE int32 GetAmmoToGive() { return AmmoToGive; };
	void KillAmmoPack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	// NN Root
	UPROPERTY(EditAnywhere)
	USceneComponent* Root;

	// NN Mesh
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* AmmoPackMesh;

	// NN How many units to rotate per frame
	float YawValue = 1.f;

	// NN Angle for, used for floating
	float Angle = 0.f;

	APawn* PlayerPawn;

	// NN Distance to activate players line trace
	UPROPERTY(EditAnywhere)
	float ActivateLineTraceDistance = 500.f;

	UPROPERTY(EditAnywhere)
	int32 AmmoToGive = 15;
};
