#include "FPSCharacter.h"
#include "FPSGameMode.h"
#include "FPSPlayerState.h"
#include "FPSWeapon.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AFPSCharacter::AFPSCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->JumpZVelocity = 700.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
	FirstPersonCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 64.0f));
	FirstPersonCamera->bUsePawnControlRotation = true;

	FirstPersonArms = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonArms"));
	FirstPersonArms->SetupAttachment(FirstPersonCamera);
	FirstPersonArms->SetOnlyOwnerSee(true);
	FirstPersonArms->bCastDynamicShadow = false;
	FirstPersonArms->CastShadow = false;

	GetMesh()->SetOwnerNoSee(true);

	MaxHealth = 100.0f;
	Health = MaxHealth;
	bDead = false;
}

void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (PC->IsLocalController())
		{
			if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
			{
				if (DefaultMappingContext)
				{
					if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
					{
						Subsystem->AddMappingContext(DefaultMappingContext, 0);
					}
				}
			}
		}
	}

	if (HasAuthority())
	{
		Health = MaxHealth;
		SpawnDefaultWeapon();
	}
}

void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
		{
			EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFPSCharacter::Move);
		}
		if (LookAction)
		{
			EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFPSCharacter::Look);
		}
		if (JumpAction)
		{
			EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &AFPSCharacter::StartJump);
			EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &AFPSCharacter::StopJump);
		}
		if (FireAction)
		{
			EnhancedInput->BindAction(FireAction, ETriggerEvent::Started, this, &AFPSCharacter::StartFire);
			EnhancedInput->BindAction(FireAction, ETriggerEvent::Completed, this, &AFPSCharacter::StopFire);
		}
		if (ReloadAction)
		{
			EnhancedInput->BindAction(ReloadAction, ETriggerEvent::Started, this, &AFPSCharacter::Reload);
		}
		if (AimAction)
		{
			EnhancedInput->BindAction(AimAction, ETriggerEvent::Started, this, &AFPSCharacter::StartAim);
			EnhancedInput->BindAction(AimAction, ETriggerEvent::Completed, this, &AFPSCharacter::StopAim);
		}
	}
}

void AFPSCharacter::Move(const FInputActionValue& Value)
{
	if (bDead)
	{
		return;
	}

	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller)
	{
		const FRotator Rotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
		const FVector Forward = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
		const FVector Right = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Forward, MovementVector.Y);
		AddMovementInput(Right, MovementVector.X);
	}
}

void AFPSCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxis = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxis.X);
	AddControllerPitchInput(LookAxis.Y);
}

void AFPSCharacter::StartJump()
{
	if (!bDead)
	{
		Jump();
	}
}

void AFPSCharacter::StopJump()
{
	StopJumping();
}

void AFPSCharacter::StartFire()
{
	if (CurrentWeapon && !bDead)
	{
		CurrentWeapon->StartFire();
	}
}

void AFPSCharacter::StopFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
	}
}

void AFPSCharacter::Reload()
{
	if (CurrentWeapon && !bDead)
	{
		CurrentWeapon->Reload();
	}
}

void AFPSCharacter::StartAim()
{
	SetAiming(true);
}

void AFPSCharacter::StopAim()
{
	SetAiming(false);
}

void AFPSCharacter::SetAiming(bool bNewAiming)
{
	if (FirstPersonCamera)
	{
		FirstPersonCamera->SetFieldOfView(bNewAiming ? 65.0f : 90.0f);
	}
}

float AFPSCharacter::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float AppliedDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (!HasAuthority() || bDead || AppliedDamage <= 0.0f)
	{
		return 0.0f;
	}

	Health = FMath::Clamp(Health - AppliedDamage, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(Health);

	if (Health <= 0.0f)
	{
		Die(EventInstigator);
	}

	return AppliedDamage;
}

void AFPSCharacter::HealToFull()
{
	if (HasAuthority())
	{
		Health = MaxHealth;
		bDead = false;
		OnHealthChanged.Broadcast(Health);
	}
}

bool AFPSCharacter::IsAlive() const
{
	return !bDead && Health > 0.0f;
}

void AFPSCharacter::EquipWeapon(AFPSWeapon* NewWeapon)
{
	if (!HasAuthority())
	{
		ServerEquipWeapon(NewWeapon);
		return;
	}

	CurrentWeapon = NewWeapon;
	AttachWeapon();
}

void AFPSCharacter::ServerEquipWeapon_Implementation(AFPSWeapon* NewWeapon)
{
	EquipWeapon(NewWeapon);
}

void AFPSCharacter::SpawnDefaultWeapon()
{
	if (!DefaultWeaponClass || CurrentWeapon)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	CurrentWeapon = GetWorld()->SpawnActor<AFPSWeapon>(DefaultWeaponClass, SpawnParams);
	AttachWeapon();
}

void AFPSCharacter::AttachWeapon()
{
	if (!CurrentWeapon)
	{
		return;
	}

	USkeletalMeshComponent* AttachMesh = FirstPersonArms.Get() ? FirstPersonArms.Get() : GetMesh();
	CurrentWeapon->SetOwner(this);
	CurrentWeapon->AttachToComponent(AttachMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("GripPoint"));
}

void AFPSCharacter::Die(AController* KillerController)
{
	bDead = true;
	StopFire();
	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OnCharacterDied.Broadcast();

	if (AFPSPlayerState* VictimState = GetPlayerState<AFPSPlayerState>())
	{
		VictimState->AddDeath();
	}

	if (KillerController && KillerController != Controller)
	{
		if (AFPSPlayerState* KillerState = KillerController->GetPlayerState<AFPSPlayerState>())
		{
			KillerState->AddKill();
		}
	}

	if (AFPSGameMode* GameMode = GetWorld()->GetAuthGameMode<AFPSGameMode>())
	{
		GameMode->HandlePlayerDeath(this, KillerController);
	}
}

void AFPSCharacter::OnRep_Health()
{
	OnHealthChanged.Broadcast(Health);
}

void AFPSCharacter::OnRep_Dead()
{
	if (bDead)
	{
		OnCharacterDied.Broadcast();
	}
}

void AFPSCharacter::OnRep_CurrentWeapon()
{
	AttachWeapon();
}

void AFPSCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSCharacter, Health);
	DOREPLIFETIME(AFPSCharacter, bDead);
	DOREPLIFETIME(AFPSCharacter, CurrentWeapon);
}
