// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPCrosshair.generated.h"

class UCanvasPanel;
class USizeBox;
class UImage;
class UFPWeaponSystemComponent;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class FPSLEARNING_API UFPCrosshair : public UUserWidget
{
	GENERATED_BODY()

public:
	UFPCrosshair(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* CrosshairCanvas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USizeBox* TopCrosshair;

	/*UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* TopFillImage;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USizeBox* BottomCrosshair;

	/*UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* BottomFillImage;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USizeBox* LeftCrosshair;

	/*UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* LeftFillImage;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USizeBox* RightCrosshair;

	/*UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* RightFillImage;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float BulletSpreadHUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float InterpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float FallingTarget;

	UFUNCTION(BlueprintCallable, Category = "Widget")
	void CrosshairUpdate();

	UFUNCTION(Category = "Widget")
	void MoveCrosshair();

	UFUNCTION(BlueprintNativeEvent, Category = "Widget")
	void TranslateCrosshair(float Target);

protected:
	ACharacter* Player;
	
	UFPWeaponSystemComponent* WeaponSystem;

	UCharacterMovementComponent* CharacterMovement;

	FTimerHandle MoveCrosshairHandle;
};
