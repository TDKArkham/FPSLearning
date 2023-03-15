// Fill out your copyright notice in the Description page of Project Settings.


#include "FPAttributeComponent.h"

#include "WeaponData.h"
#include "PhysicalMaterials/PhysicalMaterial.h"


UFPAttributeComponent::UFPAttributeComponent()
{
	MaxHealth = 100.0f;
	CurrentHealth = 100.0f;
}

void UFPAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UFPAttributeComponent::ApplyHealthChange(float Delta, FHitResult HitResult, AActor* InstigateActor)
{
	FDamageResult DamageResult;
	DamageResult.DamageLocation = HitResult.ImpactPoint;

	const float OldHealth = CurrentHealth;

	float DamageScale = 1.0f;
	if (HitResult.PhysMaterial.Get())
	{
		DamageResult.PhysicalSurfaceType = HitResult.PhysMaterial->SurfaceType;
		DamageScale = HitResult.PhysMaterial.Get()->DestructibleDamageThresholdScale;
	}

	CurrentHealth = FMath::Clamp(CurrentHealth + Delta * DamageScale, 0.0f, MaxHealth);
	const float TrueDelta = CurrentHealth - OldHealth;

	// Enemy dead.
	if (TrueDelta < 0.0f && CurrentHealth <= 0.0f)
	{
		DamageResult.HitType = EHitType::EHT_DeathHit;
	}

	if(TrueDelta != 0)
	{
		OnHealthChanged.Broadcast(InstigateActor, this, CurrentHealth, TrueDelta, DamageResult);
	}

	return Delta != 0.0f;
}

bool UFPAttributeComponent::GetIsAlive() const
{
	return CurrentHealth > 0.0f;
}
