// Fill out your copyright notice in the Description page of Project Settings.


#include "FPHUD.h"

#include "FPWeaponSystemComponent.h"
#include "Engine/Canvas.h"

void AFPHUD::BeginPlay()
{
	Super::BeginPlay();

	OwnerWeaponSystem = UFPWeaponSystemComponent::GetWeaponSystemComponent(GetOwningPawn());
}

void AFPHUD::DrawHUD()
{
	Super::DrawHUD();
	if (OwnerWeaponSystem && !OwnerWeaponSystem->GetIsAiming())
	{
		DrawRect(FColor::White, Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f, 2.0f, 2.0f);
	}
}
