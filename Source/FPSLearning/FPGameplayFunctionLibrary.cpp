// Fill out your copyright notice in the Description page of Project Settings.

#include "FPGameplayFunctionLibrary.h"

#include "FPImpactEffectBase.h"
#include "Kismet/GameplayStatics.h"

void UFPGameplayFunctionLibrary::SpawnImpactEffect(TSubclassOf<AActor> ImpactEffectClass, FHitResult HitResult)
{
	AActor* HitActor = HitResult.GetActor();
	if (ensure(ImpactEffectClass) && HitActor)
	{
		const FTransform SpawnTransform(HitResult.ImpactPoint);
		AFPImpactEffectBase* ImpactEffect = Cast<AFPImpactEffectBase>(UGameplayStatics::BeginDeferredActorSpawnFromClass(HitActor->GetWorld(), ImpactEffectClass, SpawnTransform));
		ImpactEffect->HitResult = HitResult;
		UGameplayStatics::FinishSpawningActor(ImpactEffect, SpawnTransform);
	}
}
