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
	
	// TODO: Refactor this func into Action System.
	// Do I really need to refactor this?
	UFUNCTION(BlueprintCallable, Category = "Weapon | Fire")
	void StartShooting(AFPCharacter* InstigateActor);
	
	// TODO: Refactor this func into Inventory System, the weapon shouldn't contain the info of total ammo.
	UFUNCTION(BlueprintCallable, Category = "Weapon | Ammo")
	bool AddTotalAmmo(EAmmoType AcquiredAmmoType, int32 AcquiredAmmo);

	
};
