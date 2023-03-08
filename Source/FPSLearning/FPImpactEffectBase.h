// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPImpactEffectBase.generated.h"

class USoundCue;

UCLASS()
class FPSLEARNING_API AFPImpactEffectBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFPImpactEffectBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect | Partical")
	UParticleSystem* StoneFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect | Partical")
	UParticleSystem* EnemyBodyFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect | Sound")
	USoundCue* StoneSFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect | Sound")
	USoundCue* EnemyBodySFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect | Material")
	UMaterial* ImpactDecal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect | Material")
	float DecalSize;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect | Material")
	float DecalLifespan;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect | Material")
	FRotator DecalRotation;
	
	

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Effect | Misc")
	FHitResult HitResult;
};
