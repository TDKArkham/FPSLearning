// Fill out your copyright notice in the Description page of Project Settings.


#include "FPWeaponBase.h"


AFPWeaponBase::AFPWeaponBase()
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
	SkeletalMeshComponent->SetCastShadow(false);

	SocketName = "b_RightWeapon";
}

void AFPWeaponBase::BeginPlay()
{
	Super::BeginPlay();

}

