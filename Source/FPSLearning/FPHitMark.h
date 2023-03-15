// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponData.h"
#include "FPHitMark.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class FPSLEARNING_API UFPHitMark : public UUserWidget
{
	GENERATED_BODY()
public:
	UFPHitMark(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget))
	UImage* HitMarkImage;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HitMark")
	void ActivateAnim(EHitType HitType);
};
