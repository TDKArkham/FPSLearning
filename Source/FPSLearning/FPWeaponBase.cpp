// Fill out your copyright notice in the Description page of Project Settings.


#include "FPWeaponBase.h"


AFPWeaponBase::AFPWeaponBase()
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
	SkeletalMeshComponent->SetCastShadow(false);

	/*ChamberAmmo = 1;*/
	MagazineAmmo = 30;
	TotalAmmo = 200;
	AmmoType = FText::FromString("[Auto]");

	SocketName = "b_RightWeapon";

	BulletSpread = 35.0f;
}

void AFPWeaponBase::BeginPlay()
{
	Super::BeginPlay();

}

