#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSWeapon.generated.h"

class AFPSProjectile;
class UAnimMontage;
class UAudioComponent;
class UParticleSystem;
class USkeletalMeshComponent;
class USoundBase;

UCLASS()
class UE5GISScene_API AFPSWeapon : public AActor
{
	GENERATED_BODY()

public:
	AFPSWeapon();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<USkeletalMeshComponent> Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	TSubclassOf<AFPSProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	float FireRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	float TraceDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ammo")
	int32 MagazineSize;

	UPROPERTY(ReplicatedUsing=OnRep_Ammo, BlueprintReadOnly, Category="Ammo")
	int32 CurrentAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ammo")
	int32 ReserveAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Effects")
	TObjectPtr<UParticleSystem> MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Effects")
	TObjectPtr<USoundBase> FireSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Effects")
	TObjectPtr<UAnimMontage> FireMontage;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void StartFire();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void StopFire();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Reload();

	UFUNCTION(BlueprintPure, Category="Weapon")
	bool CanFire() const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	FTimerHandle TimerHandle_AutoFire;
	float LastFireTime;

	UFUNCTION(Server, Reliable)
	void ServerStartFire();

	UFUNCTION(Server, Reliable)
	void ServerReload();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlayFireEffects();

	UFUNCTION()
	void OnRep_Ammo();

	void HandleFire();
	void FireInstantHit();
	void FireProjectile();
};
