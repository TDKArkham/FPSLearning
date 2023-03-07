// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPWeaponInfo.generated.h"

class UProgressBar;
class UTextBlock;
class ACharacter;
class UFPWeaponSystemComponent;

UENUM()
enum EBitType
{
	EBT_TwoBit,
	EBT_ThreeBit,
};

/**
 * 
 */
UCLASS()
class FPSLEARNING_API UFPWeaponInfo : public UUserWidget
{
	GENERATED_BODY()

public:
	UFPWeaponInfo(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* StaminaBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* CurrentAmmoText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ReservedAmmoText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* AmmoTypeText;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "WeaponInfo")
	FText IntToFText(int32 Num, EBitType Type);

	UFUNCTION()
	void OnAmmoChanged(int32 CurrentAmmo, int32 TotalAmmo, FText AmmoType);

protected:
	ACharacter* Player;

	UFPWeaponSystemComponent* WeaponSystem;
};
