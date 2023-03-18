// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FPGameplayFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class FPSLEARNING_API UFPGameplayFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	static void SpawnImpactEffect(TSubclassOf<AActor> ImpactEffectClass, FHitResult HitResult);
};
