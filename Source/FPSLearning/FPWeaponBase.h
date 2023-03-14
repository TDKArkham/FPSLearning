// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponData.h"
#include "FPWeaponBase.generated.h"

class UCameraComponent;
class UTimelineComponent;
class AFPCharacter;
class UFPWeaponSystemComponent;
class AFPImpactEffectBase;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UTimelineComponent* RecoilTimeline;

	FHitResult CalculateLineTrace();

	void SpawnImpactEffect(FHitResult HitResult);

	void ApplyDamageOnHitScan(FHitResult HitResult);

public:
	UPROPERTY(BlueprintReadOnly, Category = "Owner")
	AFPCharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Owner")
	UFPWeaponSystemComponent* OwnerComponent;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon | Status")
	bool bIsWeaponAiming;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon | Status | Shotgun")
	bool bIsFireIntervalActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Status | Shotgun")
	float AimBulletSpread;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Attribute | Aim")
	float XTransitionOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Attribute | Aim")
	float YTransitionOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Attribute | Aim")
	float ZTransitionOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Attribute | Aim")
	float XRotationOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Attribute | Aim")
	float YRotationOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Attribute | Aim")
	float ZRotationOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Attribute")
	EWeaponType WeaponType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Attribute")
	EAmmoType AmmoType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Attribute")
	EFireType FireType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Attribute")
	FDamageData DamageData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Attribute")
	FText AmmoTypeText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Attribute")
	float BulletSpread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Attribute")
	float ShotRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Attribute")
	float AimFieldOfView;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Attribute")
	float HipFieldOfView;
	
	/**
	 * @brief If this weapon is a shotgun, how many sub-bullet we want it to have each shoot.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Attribute | Shotgun")
	int32 PelletCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Attribute")
	int32 BulletPerMin;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Attribute")
	UCurveFloat* RecoilCurve;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Attribute")
	float VerticalRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Attribute")
	float HorizontalRecoil;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Attachment")
	FName SocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Sound")
	USoundBase* DryFireSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Animation")
	UAnimMontage* PlayerArmMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Animation")
	UAnimMontage* WeaponMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Animation")
	UAnimationAsset* FireAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Effect")
	TSubclassOf<AFPImpactEffectBase> ImpactEffectClass;
	
	// TODO: Refactor this func into Action System.
	// Do I really need to refactor this?
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon | Fire")
	void StartShooting();

	// TODO: Refactor this func into Action System.
	// Do I really need to refactor this?
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon | Fire")
	void StopShooting();

	UFUNCTION(BlueprintCallable, Category = "Weapon | Aim")
	void OnWeaponAimingEnter();

	UFUNCTION(BlueprintCallable, Category = "Weapon | Aim")
	void OnWeaponAimingExit();

	UFUNCTION(BlueprintCallable, Category = "Weapon | Fire")
	void StartRecoil();

	UFUNCTION(BlueprintCallable, Category = "Weapon | Fire")
	void StopRecoil();

	UFUNCTION(BlueprintCallable, Category = "Weapon | Fire")
	void ReverseRecoil();

	UFUNCTION(BlueprintCallable, Category = "Weapon | Fire")
	void BeginFireInterval();

	UFUNCTION()
	void ResetFireInterval();

	UFUNCTION()
	void UpdatingTimeline(float Alpha);

	UFUNCTION()
	void TimelineFinished();

	UFUNCTION()
	void TimelineFinished_TimeElapsed();
	
	// TODO: Refactor this func into Inventory System, the weapon shouldn't contain the info of total ammo.
	UFUNCTION(BlueprintCallable, Category = "Weapon | Ammo")
	bool AddTotalAmmo(EAmmoType AcquiredAmmoType, int32 AcquiredAmmo);
	
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
