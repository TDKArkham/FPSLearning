// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponData.h"
#include "FPWeaponSystemComponent.generated.h"

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

	UPROPERTY(BlueprintReadOnly, Category = "WeaponSystem | Weapon")
	AFPWeaponBase* CurrentWeapon;

	UPROPERTY(BlueprintReadOnly, Category = "WeaponSystem | Weapon")
	TArray<AFPWeaponBase*> WeaponSlots;

	UPROPERTY(BlueprintReadOnly, Category = "WeaponSystem | CharacterState")
	bool bIsSwitchingWeapon;

	UPROPERTY(BlueprintReadOnly, Category = "WeaponSystem | CharacterState")
	bool bIsReloading;

	UPROPERTY(BlueprintReadOnly, Category = "WeaponSystem | CharacterState")
	bool bCanShoot;

	UPROPERTY(BlueprintReadOnly, Category = "WeaponSystem | CharacterState")
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

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "WeaponSystem | Weapon")
	AFPWeaponBase* GetCurrentWeapon();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "WeaponSystem | Weapon")
	TArray<AFPWeaponBase*> GetWeaponSlots();

	UFUNCTION(BlueprintCallable, Category = "WeaponSystem | CharacterState")
	bool GetIsSwitchingWeapon() const;

	UFUNCTION(BlueprintCallable, Category = "WeaponSystem | CharacterState")
	bool GetIsReloading() const;

	UFUNCTION(BlueprintCallable, Category = "WeaponSystem | CharacterState")
	bool GetCanShoot() const;

	UFUNCTION(BlueprintCallable, Category = "WeaponSystem | CharacterState")
	bool GetIsAiming() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "WeaponSystem")
	static UFPWeaponSystemComponent* GetWeaponSystemComponent(AActor* TargetActor);
};
