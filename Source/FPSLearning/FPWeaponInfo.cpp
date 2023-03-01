// Fill out your copyright notice in the Description page of Project Settings.


#include "FPWeaponInfo.h"

#include "FPWeaponBase.h"
#include "FPWeaponSystemComponent.h"
#include "Components/TextBlock.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


UFPWeaponInfo::UFPWeaponInfo(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UFPWeaponInfo::NativeConstruct()
{
	Super::NativeConstruct();

	Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (Player)
	{
		WeaponSystem = UFPWeaponSystemComponent::GetWeaponSystemComponent(Player);

		if (WeaponSystem)
		{
			WeaponSystem->OnAmmoChanged.AddDynamic(this, &UFPWeaponInfo::OnAmmoChanged);

			AFPWeaponBase* CurrentWeapon = WeaponSystem->GetCurrentWeapon();
			if (CurrentWeapon)
			{
				MagazineAmmoText->SetText(IntToFText(CurrentWeapon->MagazineAmmo, EBT_TwoBit));
				ReservedAmmoText->SetText(IntToFText(CurrentWeapon->TotalAmmo, EBT_ThreeBit));
				AmmoTypeText->SetText(CurrentWeapon->AmmoTypeText);
			}
		}
	}
}

FText UFPWeaponInfo::IntToFText(int32 Num, EBitType Type)
{
	FString FormattedNum;
	if(Type == EBT_TwoBit)
	{
		FormattedNum = FString::Printf(TEXT("%02d"), Num);
	}
	else
	{
		FormattedNum = FString::Printf(TEXT("%03d"), Num);
	}
	
	return FText::FromString(FormattedNum);
}

void UFPWeaponInfo::OnAmmoChanged(int32 MagazineAmmo, int32 TotalAmmo, FText AmmoType)
{
	MagazineAmmoText->SetText(IntToFText(MagazineAmmo, EBT_TwoBit));
	ReservedAmmoText->SetText(IntToFText(TotalAmmo, EBT_ThreeBit));
	AmmoTypeText->SetText(AmmoType);
}
