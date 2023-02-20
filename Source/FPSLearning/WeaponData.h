#pragma once

UENUM(BlueprintType)
enum class ELoadOut : uint8
{
	ELO_NoWeapon		UMETA(DisplayName = "NoWeapon"),
	ELO_HasWeapon		UMETA(DisplayName = "HasWeapon"),
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_Pistol			UMETA(DisplayName = "Pistol"),
	EWT_ShotGun			UMETA(DisplayName = "ShotGun"),
	EWT_AssaultRifle	UMETA(DisplayName = "AssaultRifle"),
	EWT_SniperRifle		UMETA(DisplayName = "SniperRifle"),
	EWT_GrenadeLauncher	UMETA(DisplayName = "GrenadeLauncher"),
	EWT_RocketLauncher	UMETA(DisplayName = "RocketLauncher"),
};