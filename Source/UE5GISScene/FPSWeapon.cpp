#include "FPSWeapon.h"
#include "FPSProjectile.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"

AFPSWeapon::AFPSWeapon()
{
	bReplicates = true;
	SetReplicateMovement(true);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = Mesh;

	MuzzleSocketName = TEXT("Muzzle");
	FireRate = 0.12f;
	Damage = 20.0f;
	TraceDistance = 15000.0f;
	MagazineSize = 30;
	CurrentAmmo = MagazineSize;
	ReserveAmmo = 90;
	LastFireTime = -100.0f;
}

void AFPSWeapon::StartFire()
{
	if (!HasAuthority())
	{
		ServerStartFire();
	}

	HandleFire();
	GetWorldTimerManager().SetTimer(TimerHandle_AutoFire, this, &AFPSWeapon::HandleFire, FireRate, true);
}

void AFPSWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_AutoFire);
}

void AFPSWeapon::Reload()
{
	if (!HasAuthority())
	{
		ServerReload();
		return;
	}

	if (CurrentAmmo >= MagazineSize || ReserveAmmo <= 0)
	{
		return;
	}

	const int32 NeededAmmo = MagazineSize - CurrentAmmo;
	const int32 AmmoToLoad = FMath::Min(NeededAmmo, ReserveAmmo);
	CurrentAmmo += AmmoToLoad;
	ReserveAmmo -= AmmoToLoad;
}

bool AFPSWeapon::CanFire() const
{
	const UWorld* World = GetWorld();
	return CurrentAmmo > 0 && World && World->GetTimeSeconds() - LastFireTime >= FireRate;
}

void AFPSWeapon::ServerStartFire_Implementation()
{
	StartFire();
}

void AFPSWeapon::ServerReload_Implementation()
{
	Reload();
}

void AFPSWeapon::HandleFire()
{
	if (!CanFire())
	{
		if (CurrentAmmo <= 0)
		{
			Reload();
		}
		return;
	}

	LastFireTime = GetWorld()->GetTimeSeconds();

	if (HasAuthority())
	{
		--CurrentAmmo;
		if (ProjectileClass)
		{
			FireProjectile();
		}
		else
		{
			FireInstantHit();
		}
	}

	MulticastPlayFireEffects();
}

void AFPSWeapon::FireInstantHit()
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	AController* OwnerController = OwnerPawn ? OwnerPawn->GetController() : nullptr;

	FVector EyeLocation;
	FRotator EyeRotation;
	if (OwnerPawn)
	{
		OwnerPawn->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	}
	else
	{
		EyeLocation = GetActorLocation();
		EyeRotation = GetActorRotation();
	}

	const FVector ShotEnd = EyeLocation + EyeRotation.Vector() * TraceDistance;

	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(WeaponTrace), true, this);
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(OwnerPawn);

	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, ShotEnd, ECC_Visibility, QueryParams))
	{
		UGameplayStatics::ApplyPointDamage(Hit.GetActor(), Damage, EyeRotation.Vector(), Hit, OwnerController, this, nullptr);
	}
}

void AFPSWeapon::FireProjectile()
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	const FVector SpawnLocation = Mesh->DoesSocketExist(MuzzleSocketName) ? Mesh->GetSocketLocation(MuzzleSocketName) : GetActorLocation();
	FRotator SpawnRotation = OwnerPawn ? OwnerPawn->GetControlRotation() : GetActorRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = OwnerPawn;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	GetWorld()->SpawnActor<AFPSProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
}

void AFPSWeapon::MulticastPlayFireEffects_Implementation()
{
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, Mesh, MuzzleSocketName);
	}

	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	if (FireMontage)
	{
		if (ACharacter* CharacterOwner = Cast<ACharacter>(GetOwner()))
		{
			CharacterOwner->PlayAnimMontage(FireMontage);
		}
	}
}

void AFPSWeapon::OnRep_Ammo()
{
}

void AFPSWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSWeapon, CurrentAmmo);
}
