#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "FPSCharacter.generated.h"

class AFPSWeapon;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class USkeletalMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFPSHealthChangedSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFPSCharacterDiedSignature);

UCLASS()
class UE5GISScene_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFPSCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	TObjectPtr<UCameraComponent> FirstPersonCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Mesh")
	TObjectPtr<USkeletalMeshComponent> FirstPersonArms;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Input")
	TObjectPtr<UInputAction> FireAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Input")
	TObjectPtr<UInputAction> ReloadAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Input")
	TObjectPtr<UInputAction> AimAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	TSubclassOf<AFPSWeapon> DefaultWeaponClass;

	UPROPERTY(ReplicatedUsing=OnRep_CurrentWeapon, BlueprintReadOnly, Category="Weapon")
	TObjectPtr<AFPSWeapon> CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Health")
	float MaxHealth;

	UPROPERTY(ReplicatedUsing=OnRep_Health, BlueprintReadOnly, Category="Health")
	float Health;

	UPROPERTY(ReplicatedUsing=OnRep_Dead, BlueprintReadOnly, Category="Health")
	bool bDead;

	UPROPERTY(BlueprintAssignable, Category="Health")
	FFPSHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="Health")
	FFPSCharacterDiedSignature OnCharacterDied;

	UFUNCTION(BlueprintCallable, Category="Health")
	void HealToFull();

	UFUNCTION(BlueprintPure, Category="Health")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void EquipWeapon(AFPSWeapon* NewWeapon);

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartJump();
	void StopJump();
	void StartFire();
	void StopFire();
	void Reload();
	void StartAim();
	void StopAim();

	UFUNCTION(Server, Reliable)
	void ServerEquipWeapon(AFPSWeapon* NewWeapon);

	UFUNCTION()
	void OnRep_Health();

	UFUNCTION()
	void OnRep_Dead();

	UFUNCTION()
	void OnRep_CurrentWeapon();

	void SpawnDefaultWeapon();
	void AttachWeapon();
	void Die(AController* KillerController);
	void SetAiming(bool bNewAiming);
};
