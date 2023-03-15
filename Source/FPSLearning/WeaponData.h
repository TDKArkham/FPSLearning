#pragma once

#include "WeaponData.generated.h"

UENUM(BlueprintType)
enum class EFireType : uint8
{
	EFT_HitScan				UMETA(DisplayName = "HitScan"),
	EFT_Projectile			UMETA(DisplayName = "Projectile"),
};

UENUM(BlueprintType)
enum class ELoadOut : uint8
{
	ELO_NoWeapon			UMETA(DisplayName = "NoWeapon"),
	ELO_HasWeapon			UMETA(DisplayName = "HasWeapon"),
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_Pistol				UMETA(DisplayName = "Pistol"),
	EWT_ShotGun				UMETA(DisplayName = "ShotGun"),
	EWT_AssaultRifle		UMETA(DisplayName = "AssaultRifle"),
	EWT_SniperRifle			UMETA(DisplayName = "SniperRifle"),
	EWT_GrenadeLauncher		UMETA(DisplayName = "GrenadeLauncher"),
	EWT_RocketLauncher		UMETA(DisplayName = "RocketLauncher"),
};

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	EAT_PistolAmmo			UMETA(DisplayName = "PistolAmmo"),
	EAT_ShotGunAmmo			UMETA(DisplayName = "ShotGunAmmo"),
	EAT_AssaultRifleAmmo	UMETA(DisplayName = "AssaultRifleAmmo"),
	EAT_SniperRifleAmmo		UMETA(DisplayName = "SniperRifleAmmo"),
	EAT_GrenadeLauncherAmmo UMETA(DisplayName = "GrenadeLauncherAmmo"),
	EAT_RocketLauncherAmmo	UMETA(DisplayName = "RocketLauncherAmmo"),
};

UENUM(BlueprintType)
enum class EHitType : uint8
{
	EHT_NormalHit		UMETA(DisplayName = "NormalHit"),
	EHT_DeathHit		UMETA(DisplayName = "DeathHit")
};

USTRUCT(BlueprintType)
struct FDamageData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ImpactRadius;
};

USTRUCT(BlueprintType)
struct FDamageResult
{
	GENERATED_BODY()

	FDamageResult()
	{
		DamageLocation = FVector::ZeroVector;
		PhysicalSurfaceType = EPhysicalSurface::SurfaceType_Default;
		HitType = EHitType::EHT_NormalHit;
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector DamageLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TEnumAsByte<EPhysicalSurface> PhysicalSurfaceType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EHitType HitType;
};
