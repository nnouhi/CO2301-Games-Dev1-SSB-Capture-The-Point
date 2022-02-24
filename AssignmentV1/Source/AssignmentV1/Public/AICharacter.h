// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AICharacter.generated.h"

UCLASS()
class ASSIGNMENTV1_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacter();

	// NN When invoked shoot player
	void ShootPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SpawnDefaultController() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// NN Checks if AI character is dead
	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	// NN Get AI remaining health %
	UFUNCTION(BlueprintPure)
	float GetHealthPercentage() const;

	// NN AI character health component
	class UHealthComponent* HealthComponent;

private:
	// NN Spawn point for the projectile
	UPROPERTY(EditAnywhere)
	USceneComponent* ProjectileSpawnPoint;

	// NN The type of projectile
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AShootingProjectile> ProjectileClass;

	ACharacter* PlayerCharacterRef;

	UPROPERTY(EditAnywhere)
	float ProjectileLaunchAngle = 30.f;

	class ABaseGameMode* GameModeReference;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AController> RushingAIController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AController> DefensiveAIController;

};
