// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponData.h"
#include "Blueprint/UserWidget.h"
#include "FPDamageNumber.generated.h"

class USizeBox;
class UTextBlock;

/**
 * 
 */
UCLASS()
class FPSLEARNING_API UFPDamageNumber : public UUserWidget
{
	GENERATED_BODY()

public:
	UFPDamageNumber(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USizeBox* DamageSizeBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* DamageNumberText;

	UPROPERTY(EditAnywhere, Category = "Text | Color")
	FColor HeadShotColor;

	UPROPERTY(EditAnywhere, Category = "Text | Color")
	FColor NormalColor;

	UPROPERTY(EditAnywhere, Category = "Text | Transition")
	float XOffset;

	UPROPERTY(EditAnywhere, Category = "Text | Transition")
	float YOffset;

	UPROPERTY(EditAnywhere, Category = "Text | Transition")
	float XRandomMin;

	UPROPERTY(EditAnywhere, Category = "Text | Transition")
	float XRandomMax;

	UPROPERTY(EditAnywhere, Category = "Text | Transition")
	float YRandomMin;

	UPROPERTY(EditAnywhere, Category = "Text | Transition")
	float YRandomMax;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DamagePopUp")
	void Activate(float Damage, FDamageResult DamageResult);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DamagePopUp")
	bool bIsLocationSet;
};
