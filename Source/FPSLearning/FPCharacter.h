// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPCharacter.generated.h"

class UCameraComponent;
class UFPWeaponSystemComponent;
class UFPMainHUD;
class UTimelineComponent;

UENUM()
enum EMoveType
{
	EMT_NormalWalk		UMETA(DisplayName = "NormalWalk"),
	EMT_AimWalk			UMETA(DisplayName = "AimWalk"),
	EMT_Sprint			UMETA(DisplayName = "Sprint")
};

UCLASS()
class FPSLEARNING_API AFPCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFPCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* Camera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UFPWeaponSystemComponent* WeaponSystem;

	UPROPERTY(BlueprintReadWrite, Category = "UI")
	UFPMainHUD* MainHUD;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement | Sprint")
	UTimelineComponent* SprintTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement | Sprint")
	UCurveFloat* SprintCurve;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement | Sprint")
	bool bIsExhausted;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement | Sprint")
	bool bIsSprinting;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement | Sprint")
	float EnergyLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement | Sprint")
	float StaminaRecoverDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement | Sprint")
	float ExhaustedRecoverDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement | Speed")
	float MaxWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement | Speed")
	float MaxAimWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement | Speed")
	float MaxSprintSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement | Aim")
	float FOVInterpSpeed;

	FTimerHandle StaminaRecoverDelayHandle;
	
	UFUNCTION()
	void MoveForward(float Axis);

	UFUNCTION()
	void MoveRight(float Axis);

	UFUNCTION()
	void Fire();

	UFUNCTION()
	void StopFiring();

	UFUNCTION()
	void Aim();

	UFUNCTION()
	void StopAiming();

	UFUNCTION(Category = "Movement | Sprint")
	void UpdatingTimeline(float Value);

	UFUNCTION(Category = "Movement | Sprint")
	void TimelineFinished(float Value);

	UFUNCTION(Category = "Movement | Sprint")
	void StaminaRecoverDelayFunc();

	UFUNCTION(Category = "Movement | Sprint")
	void ExhaustedRecoverDelayFunc();
	
public:

	UFUNCTION()
	void Reload();
	
	UFUNCTION(Category = "Movement | Sprint")
	void StartSprinting();

	UFUNCTION(Category = "Movement | Sprint")
	void StopSprinting();

	UFUNCTION(Category = "Movement | Sprint")
	void Exhausted();

	UFUNCTION(BlueprintCallable, Category = "Mesh")
	float PlayAnimMontageOnArm(UAnimMontage* AnimMontage, float InPlayRate = 1.0f);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetPlayerSpeed(EMoveType MoveType);

	UFUNCTION(BlueprintCallable, Category = "Player | Camera")
	void SetFieldOfView();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Mesh")
	UCameraComponent* GetCamera();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Mesh")
	USkeletalMeshComponent* GetMeshComponent();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UI")
	UFPMainHUD* GetMainHUD();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Movement")
	bool GetIsSprinting();
};
