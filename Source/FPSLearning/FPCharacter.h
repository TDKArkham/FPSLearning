// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponData.h"
#include "GameFramework/Character.h"
#include "FPCharacter.generated.h"

class AWeaponBase;
class UCameraComponent;

UCLASS()
class FPSLEARNING_API AFPCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFPCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* Camera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	AWeaponBase* CurrentWeapon;

	UPROPERTY(BlueprintReadOnly, Category="Weapon")
	TArray<AWeaponBase*> WeaponSlots;

	bool bIsSwitchingWeapon;

	bool bIsReloading;

	bool bCanShoot;
	
	UFUNCTION()
	void MoveForward(float Axis);

	UFUNCTION()
	void MoveRight(float Axis);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	ELoadOut LoadOut;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void WeaponPickUp(TSubclassOf<AWeaponBase> WeaponToSpawn);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EquipWeapon(AWeaponBase* Weapon);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void HideAndShowWeapon(AWeaponBase* Weapon);
};
