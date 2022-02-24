// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class ASSIGNMENTV1_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE void SetbLineTrace(bool bLineTrace) { bActivateLineTrace = bLineTrace; };

	// NN Checks if player character is dead
	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	// NN Get players remaining health %
	UFUNCTION(BlueprintPure)
	float GetHealthPercentage() const;

	// NN Get players current ammo
	UFUNCTION(BlueprintPure)
	int32 GetCurrentAmmo() const;

	// NN Get players remaining total ammo
	UFUNCTION(BlueprintPure)
	int32 GetPlayersMagazinesAmmo() const;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// NN Reference to character's player controller
	class APlayerCharacterController* PlayerControllerReference;

	// NN Player character health component
	class UHealthComponent* HealthComponent;

private:
	
	// NN Scene capture for player minimap
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneCaptureComponent2D* SceneCapture2D;

	// NN Spring arm to attach scene capture to it
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SceneCaptureArm;

	// NN Sound to play when gun has no ammo and player shoots
	UPROPERTY(EditAnywhere)
	class USoundBase* GunClick;

	// NN Third person camera that is attached to spring arm
	UPROPERTY(EditAnywhere)
	class UCameraComponent* ThirdPersonCameraComp;

	// NN Spring arm that is attached to player mesh
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArmComp;

	// NN Spawn point for the projectile
	UPROPERTY(EditAnywhere)
	USceneComponent* ProjectileSpawnPoint;

	// NN The type of projectile
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AShootingProjectile> ProjectileClass;

	// NN Sensitivity for mouse Y, can be adjusted from editor
	UPROPERTY(EditAnywhere)
	float LookUpSensitivity;

	// NN Sensitivity for mouse X, can be adjusted from editor
	UPROPERTY(EditAnywhere)
	float TurnSensitivity;

	// NN The current ammo that the gun can shoot
	UPROPERTY(EditAnywhere)
	int32 CurrentMagazineAmmo = 30;

	UPROPERTY(EditAnywhere)
	int32 SingleMagazineTotalAmmo = 30;

	// NN Total ammo all the magazines have that the player has
	UPROPERTY(EditAnywhere)
	int32 PlayersMagazinesAmmo = 90;

	// NN How far to cast the linetrace 
	UPROPERTY(EditAnywhere)
	float LinetraceRange = 10000.f;

	FHitResult HitResult;

	// NN Is player able to shoot?
	bool bAbleToShoot = true;

	// NN Character moves forwards/backwards
	void MoveForward(float AxisValue);

	// NN Character moves left/right
	void MoveRight(float AxisValue);

	// NN Camera looks up at rate * sens
	void LookUpAtRate(float Rate);

	// NN Camera turns rate * sens
	void TurnAtRate(float Rate);

	// NN Character starts to sprint
	void BeginSprint();

	// NN Character stops sprinting
	void EndSprint();

	// NN Handles logic for shooting the projectile
	void Fire();

	// NN Reloads the players gun
	void StartReload();
	void EndReload();

	// NN Line traces to detected hit between
	// medkits and ammo packs
	void LineTrace();

	// NN Heal player per second
	void StartHealing(float DeltaTime);

	// NN Pause game
	void OnPause();
	
	// NN Is player able to interac?
	bool bAbleToInteract = false;

	// NN Is player able to heal?
	bool bCanHeal = false;

	bool bActivateLineTrace = false;

	void Interact();

	FString TracedObjected = TEXT("");

	class AMedKit* MedkitReference;
	class AAmmoPack* AmmoPackReference;

	float HealthToReach = 100.f;

	class ABaseGameMode* GameModeReference;
};
