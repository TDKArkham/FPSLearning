// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FPHUD.generated.h"

class UFPWeaponSystemComponent;

/**
 * 
 */
UCLASS()
class FPSLEARNING_API AFPHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	// Same as override "ReceiveDrawHUD" function in Blueprint.
	virtual void DrawHUD() override;

protected:
	UPROPERTY()
	UFPWeaponSystemComponent* OwnerWeaponSystem;
};
