// Fill out your copyright notice in the Description page of Project Settings.


#include "FPWeaponSystemComponent.h"

#include "FPCharacter.h"
#include "FPCrosshair.h"
#include "FPWeaponBase.h"
#include "WeaponData.h"
#include "FPMainHUD.h"


UFPWeaponSystemComponent::UFPWeaponSystemComponent()
{

}

void UFPWeaponSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// TODO: Optimize the logic so it won't need to Cast Owner.
	OwnerCharacter = Cast<AFPCharacter>(GetOwner());
}

void UFPWeaponSystemComponent::WeaponPickUp(TSubclassOf<AFPWeaponBase> WeaponToSpawn)
{
	if (OwnerCharacter)
	{
		WeaponSlots.Add(GetWorld()->SpawnActor<AFPWeaponBase>(WeaponToSpawn, OwnerCharacter->GetActorTransform()));
		CurrentWeapon = WeaponSlots.Last();
		if (CurrentWeapon)
		{
			const FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false);
			CurrentWeapon->AttachToComponent(OwnerCharacter->GetMeshComponent(), TransformRules, CurrentWeapon->SocketName);

			EquipWeapon(CurrentWeapon);
		}
	}

}

void UFPWeaponSystemComponent::EquipWeapon(AFPWeaponBase* Weapon)
{
	if (!bIsSwitchingWeapon)
	{
		if (bIsReloading)
		{
			bIsReloading = false;
		}

		if (CurrentWeapon != Weapon)
		{
			CurrentWeapon = Weapon;
		}

		bIsSwitchingWeapon = true;
		HideAndShowWeapon(Weapon);
		LoadOut = ELoadOut::ELO_HasWeapon;

		if (OwnerCharacter && OwnerCharacter->GetMainHUD())
		{
			OwnerCharacter->GetMainHUD()->Crosshair->CrosshairUpdate();
		}

		OnAmmoChanged.Broadcast(CurrentWeapon->MagazineAmmo, CurrentWeapon->TotalAmmo, CurrentWeapon->AmmoTypeText);

		FTimerHandle ResetTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(ResetTimerHandle, this, &UFPWeaponSystemComponent::ResetStateFlag, 0.35f);
	}
}

void UFPWeaponSystemComponent::HideAndShowWeapon(AFPWeaponBase* Weapon)
{
	for (AFPWeaponBase* WeaponInSlot : WeaponSlots)
	{
		if (WeaponInSlot)
		{
			bool bShouldHide = (WeaponInSlot != Weapon);
			WeaponInSlot->SetActorHiddenInGame(bShouldHide);
		}
	}
}

void UFPWeaponSystemComponent::ResetStateFlag()
{
	bIsSwitchingWeapon = false;
	bCanShoot = true;
}

bool UFPWeaponSystemComponent::AddAmmoToWeapon(EAmmoType AcquiredAmmoType, int32 AcquiredAmmo)
{
	for (AFPWeaponBase* Weapon : WeaponSlots)
	{
		if (Weapon && Weapon->AddTotalAmmo(AcquiredAmmoType, AcquiredAmmo))
		{
			if (Weapon == CurrentWeapon)
			{
				OnAmmoChanged.Broadcast(CurrentWeapon->MagazineAmmo, CurrentWeapon->TotalAmmo, CurrentWeapon->AmmoTypeText);
			}
			return true;
		}
	}
	return false;
}

UFPWeaponSystemComponent* UFPWeaponSystemComponent::GetWeaponSystemComponent(AActor* TargetActor)
{
	return Cast<UFPWeaponSystemComponent>(TargetActor->GetComponentByClass(StaticClass()));
}

AFPWeaponBase* UFPWeaponSystemComponent::GetCurrentWeapon()
{
	return CurrentWeapon;
}

TArray<AFPWeaponBase*> UFPWeaponSystemComponent::GetWeaponSlots()
{
	return WeaponSlots;
}

bool UFPWeaponSystemComponent::GetIsSwitchingWeapon() const
{
	return bIsSwitchingWeapon;
}

bool UFPWeaponSystemComponent::GetIsReloading() const
{
	return bIsReloading;
}

bool UFPWeaponSystemComponent::GetCanShoot() const
{
	return bCanShoot;
}

bool UFPWeaponSystemComponent::GetIsAiming() const
{
	return bIsAiming;
}
