// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASSIGNMENTV1_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();
	
	FORCEINLINE float GetCurrentHealth() { return CurrentHealth; };
	FORCEINLINE float GetMaxHealth() { return MaxHealth; };
	FORCEINLINE void SetCurrentHealth(float Amount) { CurrentHealth = Amount; };
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	bool bIsDead = false;

	UFUNCTION()
	virtual void DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* Instigator, AActor* DamageCauser);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// NN Get players health percentage
	float GetHealthPercentage() const;

private:

	float CurrentHealth = 0.f;

	// NN Default health for everyone
	UPROPERTY(EditAnywhere)
	float MaxHealth = 100.f;
};
