// Fill out your copyright notice in the Description page of Project Settings.


#include "FPAttributeComponent.h"

#include "FPWeaponBase.h"
#include "FPWeaponSystemComponent.h"
#include "WeaponData.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
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

	// Calculate damage by surface type.
	float DamageScale = 1.0f;
	if (HitResult.PhysMaterial.Get())
	{
		DamageResult.PhysicalSurfaceType = HitResult.PhysMaterial->SurfaceType;
		DamageScale = HitResult.PhysMaterial.Get()->DestructibleDamageThresholdScale;
	}

	// Calculate true delta.
	CurrentHealth = FMath::Clamp(CurrentHealth + Delta * DamageScale, 0.0f, MaxHealth);
	const float TrueDelta = CurrentHealth - OldHealth;

	// Enemy dead.
	if (TrueDelta <= 0.0f && CurrentHealth <= 0.0f)
	{
		DamageResult.HitType = EHitType::EHT_DeathHit;

		ApplyImpulse(HitResult, InstigateActor);
	}

	if (TrueDelta != 0)
	{
		OnHealthChanged.Broadcast(InstigateActor, this, CurrentHealth, TrueDelta, DamageResult);
	}

	return Delta != 0.0f;
}

void UFPAttributeComponent::ApplyImpulse(FHitResult& HitResult, AActor* InstigateActor)
{
	ACharacter* Character = Cast<ACharacter>(HitResult.GetActor());
	if (Character)
	{
		if (Character->GetMesh()->IsSimulatingPhysics())
		{
			UFPWeaponSystemComponent* WeaponSystem = UFPWeaponSystemComponent::GetWeaponSystemComponent(InstigateActor);
			if (WeaponSystem)
			{
				AFPWeaponBase* Weapon = WeaponSystem->GetCurrentWeapon();
				if (Weapon)
				{
					FVector Impulse = -HitResult.ImpactNormal * Weapon->ImpulseStrength;
					Character->GetMesh()->AddImpulseAtLocation(Impulse, HitResult.ImpactPoint, HitResult.BoneName);
				}
			}
		}
	}
}

bool UFPAttributeComponent::GetIsAlive() const
{
	return CurrentHealth > 0.0f;
}

bool UFPAttributeComponent::GetIsActorAlive(AActor* TargetActor)
{
	const UFPAttributeComponent* AttributeComponent = GetAttributeComponent(TargetActor);
	return AttributeComponent ? AttributeComponent->GetIsAlive() : false;
}

UFPAttributeComponent* UFPAttributeComponent::GetAttributeComponent(AActor* TargetActor)
{
	return TargetActor ? Cast<UFPAttributeComponent>(TargetActor->GetComponentByClass(StaticClass())) : nullptr;
}
