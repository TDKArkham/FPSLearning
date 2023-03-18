// Fill out your copyright notice in the Description page of Project Settings.

#include "FPProjectileBase.h"

#include "FPCharacter.h"
#include "FPDamageInterface.h"
#include "FPGameplayFunctionLibrary.h"
#include "FPImpactEffectBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AFPProjectileBase::AFPProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetNotifyRigidBodyCollision(true);

	RootComponent = SphereCollision;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	MeshComponent->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 10000.0f;
	ProjectileMovement->MaxSpeed = 10000.0f;
	ProjectileMovement->bInitialVelocityInLocalSpace = true;
	ProjectileMovement->ProjectileGravityScale = 0.2f;
}

void AFPProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMovement->OnProjectileStop.AddDynamic(this, &AFPProjectileBase::OnProjectileStop);
	SphereCollision->MoveIgnoreActors.Add(GetOwner());
	SphereCollision->MoveIgnoreActors.Add(GetInstigator());

	OwnerCharacter = Cast<AFPCharacter>(GetInstigator());
}

void AFPProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFPProjectileBase::OnProjectileStop(const FHitResult& ImpactResult)
{
	StopProjectile(ImpactResult);
}

void AFPProjectileBase::StopProjectile(FHitResult HitResult)
{
	if(bIsRadiusDamage)
	{
		ExplodeProjectile(HitResult);
	}
	else
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor && HitActor->Implements<UFPDamageInterface>())
		{
			IFPDamageInterface::Execute_TakeDamage(HitActor, DamageData, HitResult, OwnerCharacter);
		}
	}

	MeshComponent->SetVisibility(false);

	UFPGameplayFunctionLibrary::SpawnImpactEffect(ImpactEffectClass, HitResult);

	SetLifeSpan(0.1f);
}

void AFPProjectileBase::ExplodeProjectile(FHitResult HitResult)
{
}

void AFPProjectileBase::SetDamageData(FDamageData NewDamageData)
{
	DamageData = NewDamageData;
}
