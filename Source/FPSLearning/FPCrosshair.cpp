// Fill out your copyright notice in the Description page of Project Settings.


#include "FPCrosshair.h"

#include "FPWeaponBase.h"
#include "FPWeaponSystemComponent.h"
#include "Components/CanvasPanel.h"
#include "Components/SizeBox.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

UFPCrosshair::UFPCrosshair(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	InterpSpeed = 30.0f;

	FallingTarget = -60.0f;
}

void UFPCrosshair::NativeConstruct()
{
	Super::NativeConstruct();

	Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (Player)
	{
		WeaponSystem = UFPWeaponSystemComponent::GetWeaponSystemComponent(Player);

		CharacterMovement = Player->GetCharacterMovement();
	}


	CrosshairUpdate();
}

void UFPCrosshair::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// TODO: Optimize this with Event based broadcast.
	if (WeaponSystem)
	{
		if (WeaponSystem->GetIsAiming())
		{
			CrosshairCanvas->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			if (WeaponSystem->LoadOut == ELoadOut::ELO_HasWeapon)
			{
				CrosshairCanvas->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
}

void UFPCrosshair::CrosshairUpdate()
{
	if (WeaponSystem)
	{
		switch (WeaponSystem->LoadOut)
		{
		case ELoadOut::ELO_NoWeapon:
		{
			CrosshairCanvas->SetVisibility(ESlateVisibility::Collapsed);
			break;
		}
		case ELoadOut::ELO_HasWeapon:
		{
			CrosshairCanvas->SetVisibility(ESlateVisibility::Visible);
			break;
		}
		}

		AFPWeaponBase* Weapon = WeaponSystem->GetCurrentWeapon();

		if (Weapon)
		{
			BulletSpreadHUD = -Weapon->BulletSpread;
		}

		if (!GetWorld()->GetTimerManager().IsTimerActive(MoveCrosshairHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(MoveCrosshairHandle, this, &UFPCrosshair::MoveCrosshair, 0.01f, true);
		}
	}
}

void UFPCrosshair::MoveCrosshair()
{
	if (CharacterMovement)
	{
		if (CharacterMovement->IsFalling())
		{
			// If player is jumping.
			TranslateCrosshair(FallingTarget);
		}
		else
		{
			float Speed = Player->GetVelocity().Size();
			if (Speed > 1.0f)
			{
				// If player is walking.
				TranslateCrosshair(-Speed / 25.0f);
			}
			else
			{
				// If player is standing.
				TranslateCrosshair(0.0f);
			}
		}
	}
}

void UFPCrosshair::TranslateCrosshair_Implementation(float Target)
{
	// TODO: Seems some part can reuse, optimize the logic.

	float LocalTarget = Target + BulletSpreadHUD;
	if (TopCrosshair)
	{
		FWidgetTransform WTransform = TopCrosshair->RenderTransform;
		FVector2D Translation(WTransform.Translation.X, FMath::FInterpTo(WTransform.Translation.Y, LocalTarget, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), InterpSpeed));
		TopCrosshair->SetRenderTranslation(Translation);
	}
	if (BottomCrosshair)
	{
		FWidgetTransform WTransform = BottomCrosshair->RenderTransform;
		FVector2D Translation(WTransform.Translation.X, FMath::FInterpTo(WTransform.Translation.Y, -LocalTarget, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), InterpSpeed));
		BottomCrosshair->SetRenderTranslation(Translation);
	}
	if (LeftCrosshair)
	{
		FWidgetTransform WTransform = LeftCrosshair->RenderTransform;
		FVector2D Translation(FMath::FInterpTo(WTransform.Translation.X, LocalTarget, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), InterpSpeed), WTransform.Translation.Y);
		LeftCrosshair->SetRenderTranslation(Translation);
	}
	if (RightCrosshair)
	{
		FWidgetTransform WTransform = RightCrosshair->RenderTransform;
		FVector2D Translation(FMath::FInterpTo(WTransform.Translation.X, -LocalTarget, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), InterpSpeed), WTransform.Translation.Y);
		RightCrosshair->SetRenderTranslation(Translation);
	}
}
