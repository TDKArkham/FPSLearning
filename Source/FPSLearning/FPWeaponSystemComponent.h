// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponData.h"
#include "FPWeaponSystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAmmoChanged, int32, MagazineAmmo, int32, TotalAmmo, FText, AmmoType);

class AFPWeaponBase;
class AFPCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FPSLEARNING_API UFPWeaponSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFPWeaponSystemComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, Category = "WeaponSystem | Weapon")
	AFPWeaponBase* CurrentWeapon;

	UPROPERTY(BlueprintReadOnly, Category = "WeaponSystem | Weapon")
	TArray<AFPWeaponBase*> WeaponSlots;

	UPROPERTY(BlueprintReadWrite, Category = "WeaponSystem | CharacterState")
	bool bIsSwitchingWeapon;

	UPROPERTY(BlueprintReadWrite, Category = "WeaponSystem | CharacterState")
	bool bIsReloading;

	UPROPERTY(BlueprintReadWrite, Category = "WeaponSystem | CharacterState")
	bool bCanShoot;

	UPROPERTY(BlueprintReadWrite, Category = "WeaponSystem | CharacterState")
	bool bCanReload;

	UPROPERTY(BlueprintReadWrite, Category = "WeaponSystem | CharacterState")
	bool bIsAiming;

	AFPCharacter* OwnerCharacter;

	UFUNCTION()
	void ResetStateFlag();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponSystem")
	ELoadOut LoadOut;

	UFUNCTION(BlueprintCallable, Category = "WeaponSystem")
	void WeaponPickUp(TSubclassOf<AFPWeaponBase> WeaponToSpawn);

	UFUNCTION(BlueprintCallable, Category = "WeaponSystem")
	void EquipWeapon(AFPWeaponBase* Weapon);

	UFUNCTION(BlueprintCallable, Category = "WeaponSystem")
	void HideAndShowWeapon(AFPWeaponBase* Weapon);

	UPROPERTY(BlueprintAssignable, Category = "WeaponSystem | Delegate")
	FOnAmmoChanged OnAmmoChanged;
	
	UFUNCTION(BlueprintCallable, Category = "WeaponSystem | Weapon")
	bool AddAmmoToWeapon(EAmmoType AcquiredAmmoType, int32 AcquiredAmmo);

	UFUNCTION(BlueprintCallable, Category = "WeaponSystem | Weapon")
	void FireWeapon();

	UFUNCTION(BlueprintCallable, Category = "WeaponSystem | Weapon")
	void StopShooting();

	UFUNCTION(BlueprintCallable, Category = "WeaponSystem | Weapon")
	void ReloadWeapon();

	UFUNCTION()
	void ReloadWeapon_TimeElapsed();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "WeaponSystem | Weapon")
	AFPWeaponBase* GetCurrentWeapon();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "WeaponSystem | Weapon")
	TArray<AFPWeaponBase*> GetWeaponSlots();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "WeaponSystem | CharacterState")
	bool GetIsSwitchingWeapon() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "WeaponSystem | CharacterState")
	bool GetIsReloading() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "WeaponSystem | CharacterState")
	bool GetCanShoot() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "WeaponSystem | CharacterState")
	bool GetIsAiming() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "WeaponSystem")
	static UFPWeaponSystemComponent* GetWeaponSystemComponent(AActor* TargetActor);
};
