// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPCharacter.generated.h"

class UCameraComponent;
class UFPWeaponSystemComponent;
class UFPMainHUD;

UCLASS()
class FPSLEARNING_API AFPCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFPCharacter();

protected:
	virtual void BeginPlay() override;

public:	
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
	
	UFUNCTION()
	void MoveForward(float Axis);

	UFUNCTION()
	void MoveRight(float Axis);

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Mesh")
	USkeletalMeshComponent* GetMeshComponent();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UI")
	UFPMainHUD* GetMainHUD();
};
