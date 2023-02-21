// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponData.h"
#include "FPWeaponSystemComponent.generated.h"

class AFPWeaponBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FPSLEARNING_API UFPWeaponSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFPWeaponSystemComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category = "WeaponSystem")
	AFPWeaponBase* CurrentWeapon;

	UPROPERTY(BlueprintReadOnly, Category = "WeaponSystem")
	TArray<AFPWeaponBase*> WeaponSlots;

	bool bIsSwitchingWeapon;

	bool bIsReloading;

	bool bCanShoot;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponSystem")
	ELoadOut LoadOut;

	UFUNCTION(BlueprintCallable, Category = "WeaponSystem")
	void WeaponPickUp(TSubclassOf<AFPWeaponBase> WeaponToSpawn);

	UFUNCTION(BlueprintCallable, Category = "WeaponSystem")
	void EquipWeapon(AFPWeaponBase* Weapon);

	UFUNCTION(BlueprintCallable, Category = "WeaponSystem")
	void HideAndShowWeapon(AFPWeaponBase* Weapon);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "WeaponSystem")
	static UFPWeaponSystemComponent* GetWeaponSystemComponent(AActor* TargetActor);
};
