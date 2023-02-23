// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPMainHUD.generated.h"

class UFPCrosshair;

/**
 * 
 */
UCLASS()
class FPSLEARNING_API UFPMainHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UFPMainHUD(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UFPCrosshair* Crosshair;
};
