// Fill out your copyright notice in the Description page of Project Settings.


#include "FPHUD.h"

#include "Engine/Canvas.h"

void AFPHUD::DrawHUD()
{
	Super::DrawHUD();

	DrawRect(FColor::White, Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f, 2.0f, 2.0f);
}
