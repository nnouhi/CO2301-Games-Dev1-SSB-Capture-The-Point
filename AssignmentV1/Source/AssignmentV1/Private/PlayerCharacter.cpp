// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "BaseGameMode.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerCharacterController.h"
#include "Engine/SkeletalMeshSocket.h"
#include "ShootingProjectile.h"
#include "Sound/SoundBase.h"
#include "Kismet/Gameplaystatics.h"
#include "Components/SceneCaptureComponent2D.h"
#include "DrawDebugHelpers.h"
#include "MedKit.h"
#include "AmmoPack.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HealthComponent.h"
#include "..\Public\PlayerCharacter.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// NN Instantiate class components
	ThirdPersonCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCameraComp"));
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	SceneCapture2D = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Minimap"));
	SceneCaptureArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Minimap Arm"));
	

	// NN Attaching class components
	SpringArmComp->SetupAttachment(GetMesh());
	ThirdPersonCameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	ProjectileSpawnPoint->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Muzzle_01"));
	SceneCaptureArm->SetupAttachment(GetMesh());
	SceneCapture2D->SetupAttachment(SceneCaptureArm);
	// NN NOTE: Muzzle_01 was created on Wraith's skeletal mesh

	// NN Setting class components attributes
	SpringArmComp->TargetArmLength = 150.f;
	SpringArmComp->bUsePawnControlRotation = true;
	ThirdPersonCameraComp->bUsePawnControlRotation = false;
	ProjectileSpawnPoint->SetRelativeLocation(FVector(100.f, 0.f, 0.f));
	/* NN NOTE : 50 units in front of player in case player is sprintingand shooting;
	 * projectile may collide with player when spawned
	 */
	AddOwnedComponent(HealthComponent); // add component to character

	MedkitReference = NULL;
	AmmoPackReference = NULL;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerControllerReference = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(this, 0));
	
	AShootingProjectile* ShootingProjectileReference = Cast<AShootingProjectile>(ProjectileClass);
	
	GameModeReference = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (GameModeReference)
	{
		TurnSensitivity = GameModeReference->GetBaseTurnRate();
		LookUpSensitivity = TurnSensitivity;
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// NN Boolean is set by MedKit
	if (bActivateLineTrace)
	{
		LineTrace();
	}
	else
	{
		if (PlayerControllerReference)
		{
			// NN Hide widget because player is not close to interactable object
			PlayerControllerReference->HideInteractableWidget(); 
		}
	}

	if (bCanHeal)
	{
		StartHealing(DeltaTime);
	}
}

bool APlayerCharacter::IsDead() const
{
	if (HealthComponent)
	{
		return HealthComponent->GetCurrentHealth() <= 0.f;
	}
	return false;
}

float APlayerCharacter::GetHealthPercentage() const
{

	if (HealthComponent)
	{
		return HealthComponent->GetHealthPercentage();
	}

	return 0.f;
}

int32 APlayerCharacter::GetCurrentAmmo() const
{
	return CurrentMagazineAmmo;
}

int32 APlayerCharacter::GetPlayersMagazinesAmmo() const
{
	return PlayersMagazinesAmmo;
}


void APlayerCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void APlayerCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void APlayerCharacter::LookUpAtRate(float Rate)
{
	// NN Adjust looking up rate by sensitivity
	AddControllerPitchInput(Rate * LookUpSensitivity * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::TurnAtRate(float Rate)
{
	// NN Adjust turning rate by sensitivity
	AddControllerYawInput(Rate * TurnSensitivity * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::BeginSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void APlayerCharacter::EndSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

void APlayerCharacter::Fire()
{
	if (CurrentMagazineAmmo != 0 && bAbleToShoot)
	{
		if (ProjectileSpawnPoint)
		{
			// NN Get spawnpoints (SceneComponent) location and rotation
			FVector ProjectileSpawnPointLocation = ProjectileSpawnPoint->GetComponentLocation();
			FRotator ProjectileSpawnPointRotation = ProjectileSpawnPoint->GetComponentRotation();

			if (ProjectileClass)
			{
				AShootingProjectile* Projectile = GetWorld()->SpawnActor<AShootingProjectile>(ProjectileClass, ProjectileSpawnPointLocation, ProjectileSpawnPointRotation);
				Projectile->SetOwner(this); // set the owner to the actor that fired it
				Projectile->SetProjectileDamage(20.f);
				CurrentMagazineAmmo--;
				/*UE_LOG(LogTemp, Warning, TEXT("Current Ammo: %i"), CurrentMagazineAmmo);
				UE_LOG(LogTemp, Warning, TEXT("Players Magazines Ammo: %i"), PlayersMagazinesAmmo);*/
			}
		}
	}
	else
	{
		if (GunClick)
		{
			UGameplayStatics::PlaySoundAtLocation(this, GunClick, GetActorLocation());
		}
	}
	
}

void APlayerCharacter::EndReload()
{
}

void APlayerCharacter::LineTrace()
{
	FVector StartLocation = ThirdPersonCameraComp->GetComponentLocation();
	FVector ForwardVector = ThirdPersonCameraComp->GetForwardVector();

	FVector EndLocation = (ForwardVector * LinetraceRange) + StartLocation; //NOTE: Change from editor
	
	//DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Blue, 1.0f); //remove later

	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility))
	{
		if (HitResult.GetActor()) // typesafe (hitting some objects returns nullptr)
		{
			if (HitResult.GetActor()->IsA(AMedKit::StaticClass()))
			{
				UE_LOG(LogTemp, Warning, TEXT("MEDKIT"));
				if (HealthComponent->GetCurrentHealth() != HealthComponent->GetMaxHealth())
				{
					MedkitReference = Cast<AMedKit>(HitResult.GetActor());
					TracedObjected = TEXT("Medkit");
					bAbleToInteract = true;
				}
			}
			else if (HitResult.GetActor()->IsA(AAmmoPack::StaticClass()))
			{
				AmmoPackReference = Cast<AAmmoPack>(HitResult.GetActor());
				TracedObjected = TEXT("AmmoPack");
				bAbleToInteract = true;
			}
			else
			{
				bAbleToInteract = false;
				PlayerControllerReference->HideInteractableWidget();
				bActivateLineTrace = false;
				return;
			}
			PlayerControllerReference->DisplayInteractableWidget();
		}
	}
}

void APlayerCharacter::StartHealing(float DeltaTime)
{
	if (bCanHeal)
	{
		// NN Heal over time till current health reaches the HealthToReach
		if (HealthComponent && HealthComponent->GetCurrentHealth() < HealthToReach)
		{
			float CurrentPlayerHealth = HealthComponent->GetCurrentHealth();
			CurrentPlayerHealth += DeltaTime * MedkitReference->GetHealingPerSecond();
			HealthComponent->SetCurrentHealth(CurrentPlayerHealth);
		}
		else
		{
			bCanHeal = false;
		}
	}
}

void APlayerCharacter::OnPause()
{
	if (GameModeReference)
	{
		GameModeReference->PauseGame();
	}
}

void APlayerCharacter::Interact()
{
	// NN Check which object was line traced 
	if (bAbleToInteract)
	{
		if (TracedObjected == TEXT("Medkit"))
		{
			// NN Get the health you have to reach with healing
			HealthToReach = FMath::Min(HealthComponent->GetCurrentHealth() + MedkitReference->GetHPToRestore(), HealthComponent->GetMaxHealth());
			MedkitReference->KillMedkit(); // NN Destroy medkit
			bCanHeal = true; // activate healing per second
			bAbleToInteract = false;
		}
		else if (TracedObjected == TEXT("AmmoPack"))
		{
			PlayersMagazinesAmmo += AmmoPackReference->GetAmmoToGive();
			AmmoPackReference->KillAmmoPack();
			bAbleToInteract = false;
		}
	}
}

void APlayerCharacter::StartReload()
{

	bAbleToShoot = false;

	// NN If player reloads and the gun is full
	if (CurrentMagazineAmmo == SingleMagazineTotalAmmo)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ammo is full"));
		bAbleToShoot = true;
		return;
	}

	// NN Find out how many bullets the mag is missing
	int32 AmmoToAdd = SingleMagazineTotalAmmo - CurrentMagazineAmmo; //max mag capacity - current ammo

	// NN If the ammo to add is more than the player has, add the remaining ammo to the mag
	if (AmmoToAdd >= PlayersMagazinesAmmo)
	{
		AmmoToAdd = PlayersMagazinesAmmo;
		CurrentMagazineAmmo += AmmoToAdd;
		PlayersMagazinesAmmo = 0;
	}
	else if(PlayersMagazinesAmmo!=0)
	{
		// NN Add bullets to magazine
		CurrentMagazineAmmo += AmmoToAdd;

		// NN Remove the added ammo from the total players ammo
		PlayersMagazinesAmmo -= AmmoToAdd;
	}
	bAbleToShoot = true;
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// NN Axis mappings
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APlayerCharacter::LookUpAtRate);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APlayerCharacter::TurnAtRate);

	// NN Action mappings
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &APlayerCharacter::BeginSprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &APlayerCharacter::EndSprint);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &APlayerCharacter::Fire);
	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &APlayerCharacter::StartReload);
	PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &APlayerCharacter::Interact);
	PlayerInputComponent->BindAction(TEXT("Pause"), IE_Pressed, this, &APlayerCharacter::OnPause);

}
