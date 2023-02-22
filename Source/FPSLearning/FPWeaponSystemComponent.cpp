// Fill out your copyright notice in the Description page of Project Settings.


#include "FPWeaponSystemComponent.h"

#include "FPCharacter.h"
#include "FPWeaponBase.h"
#include "WeaponData.h"


UFPWeaponSystemComponent::UFPWeaponSystemComponent()
{

}

void UFPWeaponSystemComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UFPWeaponSystemComponent::WeaponPickUp(TSubclassOf<AFPWeaponBase> WeaponToSpawn)
{
	// TODO: Optimize the logic so it won't need to Cast Owner.
	AFPCharacter* OwnerCharacter = Cast<AFPCharacter>(GetOwner());
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

		// TODO: Add a delay here to reset bIsSwitchingWeapon & bCanShoot state.
		bIsSwitchingWeapon = false;
		bCanShoot = true;
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

UFPWeaponSystemComponent* UFPWeaponSystemComponent::GetWeaponSystemComponent(AActor* TargetActor)
{
	return Cast<UFPWeaponSystemComponent>(TargetActor->GetComponentByClass(StaticClass()));
}
