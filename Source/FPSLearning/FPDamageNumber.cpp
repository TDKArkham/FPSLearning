// Fill out your copyright notice in the Description page of Project Settings.

#include "FPDamageNumber.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

UFPDamageNumber::UFPDamageNumber(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsLocationSet = false;

	XOffset = 50.0f;
	YOffset = 100.0f;
	XRandomMin = -25.0f;
	XRandomMax = 50.0f;
	YRandomMin = -30.0f;
	YRandomMax = 50.0f;
}

void UFPDamageNumber::Activate_Implementation(float Damage, FDamageResult DamageResult)
{
	if (DamageNumberText)
	{
		DamageNumberText->SetText(FText::FromString(FString::FromInt(FMath::RoundToInt(FMath::Abs(Damage)))));
		const FColor TextColor = DamageResult.PhysicalSurfaceType == EPhysicalSurface::SurfaceType1 ? HeadShotColor : NormalColor;
		DamageNumberText->SetColorAndOpacity(FSlateColor(TextColor));
	}

	if (DamageSizeBox)
	{
		FVector2D ScreenPosition;
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC)
		{
			const bool bIsOnScreen = UGameplayStatics::ProjectWorldToScreen(PC, DamageResult.DamageLocation, ScreenPosition);
			if(bIsOnScreen)
			{
				const float ScreenScale = UWidgetLayoutLibrary::GetViewportScale(this);
				ScreenPosition /= ScreenScale;
			}
		}
		ScreenPosition.X = ScreenPosition.X + XOffset + FMath::FRandRange(XRandomMin, XRandomMax);
		ScreenPosition.Y = ScreenPosition.Y + YOffset + FMath::FRandRange(YRandomMin, YRandomMax);
		DamageSizeBox->SetRenderTranslation(ScreenPosition);
	}

}
