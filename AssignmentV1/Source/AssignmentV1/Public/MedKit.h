// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MedKit.generated.h"

UCLASS()
class ASSIGNMENTV1_API AMedKit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMedKit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	// NN Destroy
	void KillMedkit();

	FORCEINLINE void SetHPToRestore(float amount) { HPToRestore /= amount; };

	FORCEINLINE float GetHealingPerSecond() { return HealingPerSecond; };
	FORCEINLINE float GetHPToRestore() { return HPToRestore; };
private:

	// NN Root
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	// NN Mesh
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MedkitMesh;

	// NN How many units to rotate per frame
	float YawValue = 1.f;

	// NN Angle for, used for floating
	float Angle = 0.f;

	// NN Distance to activate players line trace
	UPROPERTY(EditAnywhere)
	float ActivateLineTraceDistance = 500.f;

	APawn* PlayerPawn;
	class APlayerCharacter* PlayerPawnReference;
	
	// NN Regen rate
	float HealingPerSecond = 50.f;

	// NN How much health it provides
	UPROPERTY(EditAnywhere)
	float HPToRestore = 40.f;
};
