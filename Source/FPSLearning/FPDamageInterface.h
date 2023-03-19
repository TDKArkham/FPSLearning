// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponData.h"
#include "UObject/Interface.h"
#include "FPDamageInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UFPDamageInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FPSLEARNING_API IFPDamageInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage")
	bool TakeDamage(FDamageData DamageData, FHitResult HitResult, AActor* InstigateActor);

};
