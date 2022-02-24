// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CapturePoint.generated.h"

UCLASS()
class ASSIGNMENTV1_API ACapturePoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACapturePoint();

	FORCEINLINE int32 GetPlayersCapturePercentage() { return PlayersCapturePercentange; };
	FORCEINLINE int32 GetAIsCapturePercentange() { return AIsCapturePercentange; };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	// NN Collision box
	UPROPERTY(EditAnywhere)
	class UBoxComponent* CollisionBox;

	// NN Root 
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	// NN Mesh
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* FlagMesh;

	// NN Handles collisions with AI Character && Player character
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp,AActor* OtherActor,
		UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,
		const FHitResult& SweepResult);

	// NN Handles end of collision with AI Character && Player character
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// NN How fast the flag goes down
	UPROPERTY(EditAnywhere)
	float FlagSpeed = -50.f;

	FVector StartingLocation;
	FVector NewLocation;

	int32 NumOfCharactersInArea = 0;
	int32 PreviousNumOfCharactersInArea=0;

	float CurrentFlagLocation;
	float StartingFlagLocation;
	float EndingFlagLocation;

	bool bPlayerDefendsHisFlag = false;
	bool bAIDefendsHisFlag = false;

	class ABaseGameMode* GameModeReference;

	int32 PlayersCapturePercentange = 0;
	int32 AIsCapturePercentange = 0;
};
