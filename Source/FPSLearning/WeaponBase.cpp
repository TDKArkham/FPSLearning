// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"


AWeaponBase::AWeaponBase()
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
	SkeletalMeshComponent->SetCastShadow(false);

	SocketName = "b_RightWeapon";
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

