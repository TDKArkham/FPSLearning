// Fill out your copyright notice in the Description page of Project Settings.


#include "FPAttributeComponent.h"

#include "PhysicalMaterials/PhysicalMaterial.h"


UFPAttributeComponent::UFPAttributeComponent()
{
	MaxHealth = 100.0f;
	CurrentHealth = 100.0f;
}

bool UFPAttributeComponent::ApplyHealthChange(float Delta, FHitResult HitResult, AActor* InstigateActor)
{
	const float OldHealth = CurrentHealth;
	const float DamageScale = HitResult.PhysMaterial.Get() ? HitResult.PhysMaterial.Get()->DestructibleDamageThresholdScale : 1.0f;
	CurrentHealth = FMath::Clamp(CurrentHealth + Delta * DamageScale, 0.0f, MaxHealth);
	const float TrueDelta = CurrentHealth - OldHealth;

	// DEBUG: Remove this code later.
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, FString::SanitizeFloat(TrueDelta));

	if (TrueDelta < 0.0f && CurrentHealth <= 0.0f)
	{
		// TODO: Character Dead.
	}

	return TrueDelta != 0.0f;
}

bool UFPAttributeComponent::GetIsAlive() const
{
	return CurrentHealth > 0.0f;
}
