// Fill out your copyright notice in the Description page of Project Settings.


#include "FPImpactEffectBase.h"

#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Sound/SoundCue.h"


AFPImpactEffectBase::AFPImpactEffectBase()
{
	DecalLifespan = 10.0f;

}

void AFPImpactEffectBase::BeginPlay()
{
	Super::BeginPlay();

	if (HitResult.bBlockingHit)
	{
		// Check the physical surface type, if it is Stone(Type7) or Default, set the emitter to StoneFX and set the sound to StoneSFX, if it is other types such as
		// Head(Type1)、Chest(Type2) and so on, then we set the emitter to EnemyBodyFX and set the sound to EnemyBodySFX
		UParticleSystem* EmitterTemplate;
		USoundCue* Sound;
		const EPhysicalSurface Surface = UPhysicalMaterial::DetermineSurfaceType(HitResult.PhysMaterial.Get());
		if (Surface == EPhysicalSurface::SurfaceType_Default || Surface == EPhysicalSurface::SurfaceType7)
		{
			EmitterTemplate = StoneFX;
			Sound = StoneSFX;
		}
		else
		{
			EmitterTemplate = EnemyBodyFX;
			Sound = EnemyBodySFX;
		}

		const UWorld* World = GetWorld();
		const FVector SpawnLocation = HitResult.ImpactPoint;
		const FRotator TempRot = HitResult.Normal.ToOrientationRotator();
		DecalRotation = FRotator(TempRot.Pitch, TempRot.Yaw, FMath::FRandRange(-100.0f, 100.0f));

		UGameplayStatics::SpawnEmitterAtLocation(World, EmitterTemplate, SpawnLocation);
		UGameplayStatics::PlaySoundAtLocation(World, Sound, SpawnLocation);
		UGameplayStatics::SpawnDecalAtLocation(World, ImpactDecal, FVector(DecalSize), SpawnLocation, DecalRotation, DecalLifespan);
		UGameplayStatics::SpawnDecalAttached(ImpactDecal, FVector(DecalSize), HitResult.Component.Get(), HitResult.BoneName,
											 SpawnLocation, DecalRotation, EAttachLocation::KeepRelativeOffset, DecalLifespan);
	}

	if (DecalLifespan <= 0.0f)
	{
		SetLifeSpan(0.1f);
	}
	SetLifeSpan(DecalLifespan + 0.1f);
}


