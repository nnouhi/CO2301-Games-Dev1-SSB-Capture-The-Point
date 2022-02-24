// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ShootingProjectile.h"
#include "BaseGameMode.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "AIController.h"



// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));


	ProjectileSpawnPoint->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Muzzle_01"));
	ProjectileSpawnPoint->SetRelativeLocation(FVector(50.f, 0.f, 0.f));

	AddOwnedComponent(HealthComponent); // add component to character
}

void AAICharacter::ShootPlayer()
{
	if (PlayerCharacterRef)
	{
		if (ProjectileClass)
		{
			// NN Find location's of player and spawnpoint
			FVector PlayerLocation = PlayerCharacterRef->GetActorLocation();
			FVector ProjectileLocation = ProjectileSpawnPoint->GetComponentLocation();

			FRotator ProjectileRotation = UKismetMathLibrary::FindLookAtRotation(ProjectileLocation, PlayerLocation);

			// NN == Calculating Projectile Velocity for Parabolic Arc having fixed angle == 
			// https://forums.unrealengine.com/t/calculating-projectile-velocity-for-parabolic-arc/90351

			const float Gravity = GetWorld()->GetGravityZ() * -1; // gravity must be positive
			float Theta = (ProjectileLaunchAngle * PI / 180); // convert to rad

			FVector direction = PlayerLocation - ProjectileLocation; //direction

			float Sz = direction.Z;// height difference
			direction.Z = 0; // remove hight from direction
			float Sx = direction.Size();// distance

			const float V = (Sx / cos(Theta)) * FMath::Sqrt((Gravity * 1) / (2 * (Sx * tan(Theta) - Sz)));
			FVector VelocityOutput = FVector(V * cos(Theta), 0, V * sin(Theta));

			// NN Spawn projectile using new yaw 
			AShootingProjectile* Projectile = GetWorld()->SpawnActor<AShootingProjectile>(
				ProjectileClass, 
				ProjectileLocation,
				FRotator(0.f, ProjectileRotation.Yaw, 0)
				);

			if (GameModeReference)
			{
				Projectile->SetProjectileDamage(20.f * GameModeReference->GetDifficulty());
			}
			// NN Apply new velocity 
			Projectile->ProjectileMovementComponent->SetVelocityInLocalSpace(VelocityOutput);
			Projectile->SetOwner(this);
		}
	}
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerCharacterRef = UGameplayStatics::GetPlayerCharacter(this, 0);
	GameModeReference = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	/*UE_LOG(LogTemp, Warning, TEXT("Character Created"));*/
}

void AAICharacter::SpawnDefaultController()
{

	if (Controller != nullptr || GetNetMode() == NM_Client)
	{
		return;
	}
	
	if (AIControllerClass != nullptr)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Instigator = GetInstigator();
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnInfo.OverrideLevel = GetLevel();
		SpawnInfo.ObjectFlags |= RF_Transient;

		// NN Randomly choose which controller to give to AI
		if (FMath::RandBool())
		{
			AController* NewController = GetWorld()->SpawnActor<AController>(RushingAIController, GetActorLocation(), GetActorRotation(), SpawnInfo);
			if (NewController != nullptr)
			{
				NewController->Possess(this);
			}
		}
		else
		{
			AController* NewController = GetWorld()->SpawnActor<AController>(DefensiveAIController, GetActorLocation(), GetActorRotation(), SpawnInfo);
			if (NewController != nullptr)
			{
				NewController->Possess(this);
			}
		}
		
	}
}

// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool AAICharacter::IsDead() const
{
	if (HealthComponent)
	{
		return HealthComponent->GetCurrentHealth() <= 0.f;
	}
	return false;
}

float AAICharacter::GetHealthPercentage() const
{

	if (HealthComponent)
	{

		return HealthComponent->GetHealthPercentage();
	}

	return 0.f;
}

