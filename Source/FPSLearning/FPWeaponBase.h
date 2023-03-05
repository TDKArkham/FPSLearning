// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponData.h"
#include "FPWeaponBase.generated.h"

class UCameraComponent;

UCLASS()
class FPSLEARNING_API AFPWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	AFPWeaponBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* SkeletalMeshComponent;

	FHitResult CalculateLineTrace(AFPCharacter* Player);

public:
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Status")
	int32 ChamberAmmo;*/
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Status")
	int32 MagazineAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Status")
	int32 TotalAmmo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon | Status")
	int32 CurrentAmmo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon | Status")
	int32 AmmoDifference;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon | Status")
	bool bIsShooting;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Attribute")
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Attribute")
	EAmmoType AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Attribute")
	EFireType FireType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Attribute")
	FText AmmoTypeText;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon | Attribute")
	float BulletSpread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Attribute")
	float ShotRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Attachment")
	FName SocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Sound")
	USoundBase* DryFireSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Animation")
	UAnimMontage* PlayerArmMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Animation")
	UAnimMontage* WeaponMontage;
	
	// TODO: Refactor this func into Action System.
	// Do I really need to refactor this?
	UFUNCTION(BlueprintCallable, Category = "Weapon | Fire")
	void StartShooting(AFPCharacter* InstigateActor);


	// TODO: Refactor this func into Action System.
	// Do I really need to refactor this?
	UFUNCTION(BlueprintCallable, Category = "Weapon | Fire")
	void StopShooting(/*AFPCharacter* InstigateActor*/);
	
	// TODO: Refactor this func into Inventory System, the weapon shouldn't contain the info of total ammo.
	UFUNCTION(BlueprintCallable, Category = "Weapon | Ammo")
	bool AddTotalAmmo(EAmmoType AcquiredAmmoType, int32 AcquiredAmmo);

	UFUNCTION(BlueprintCallable, Category = "Weapon | Ammo")
	void AutoReloadOnEmpty(AFPCharacter* InstigateActor);

	UFUNCTION()
	void AutoReloadOnEmpty_TimeElapsed(AFPCharacter* InstigateActor);
	
	UFUNCTION(BlueprintCallable, Category = "Weapon | Ammo")
	void ReloadCalculate();

	UFUNCTION(BlueprintCallable, Category = "Mesh")
	float PlayAnimMontageOnWeapon(float InPlayRate = 1.0f);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon | Ammo")
	bool IsMagazineFull();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon | Ammo")
	bool HasLoadedAmmo();
	
	/**
	 * @brief Check if the player has more reserved ammo to use.
	 * @return True if there is left, False if there is not
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon | Ammo")
	bool HasReservedAmmo();
};
