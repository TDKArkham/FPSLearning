// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponData.h"
#include "GameFramework/Actor.h"
#include "FPProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class AFPImpactEffectBase;
class AFPCharacter;

UCLASS()
class FPSLEARNING_API AFPProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	AFPProjectileBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	bool bIsRadiusDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	TSubclassOf<AFPImpactEffectBase> ImpactEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	FDamageData DamageData;

	UPROPERTY()
	AFPCharacter* OwnerCharacter;

	UFUNCTION()
	void OnProjectileStop(const FHitResult& ImpactResult);

	UFUNCTION()
	void StopProjectile(FHitResult HitResult);

	UFUNCTION()
	void ExplodeProjectile(FHitResult HitResult);

public:

	void SetDamageData(FDamageData NewDamageData);
};
