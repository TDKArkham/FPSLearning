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
	SphereCollision->SetCollisionProfileName("Projectile");

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

	SphereCollision->OnComponentHit.AddDynamic(this, &AFPProjectileBase::OnComponentHit);
	SphereCollision->MoveIgnoreActors.Add(GetOwner());
	SphereCollision->MoveIgnoreActors.Add(GetInstigator());

	OwnerCharacter = Cast<AFPCharacter>(GetInstigator());
}

void AFPProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFPProjectileBase::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
									   UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	StopProjectile(Hit);
}

void AFPProjectileBase::StopProjectile(FHitResult HitResult)
{
	if (bIsRadiusDamage)
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

	FinishDamage(HitResult);
}

void AFPProjectileBase::ExplodeProjectile(FHitResult HitResult)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery2);		// WorldDynamic
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery4);		// PhysicsBody

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetOwner());

	TArray<FHitResult> HitResults;

	bool bIsHit = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), HitResult.ImpactPoint, HitResult.ImpactPoint,
																   DamageData.ImpactRadius, ObjectTypes, false, ActorsToIgnore,
																   EDrawDebugTrace::Type::None, HitResults, true);
	if (bIsHit)
	{
		AActor* HitActor = nullptr;
		TArray<AActor*> HitActors;
		for (FHitResult NewHitResult : HitResults)
		{
			if (HitActor == NewHitResult.GetActor() || HitActors.Contains(NewHitResult.GetActor()))
			{
				continue;
			}
			HitActor = NewHitResult.GetActor();
			HitActors.Add(HitActor);
			if (HitActor && HitActor->Implements<UFPDamageInterface>())
			{
				IFPDamageInterface::Execute_TakeDamage(HitActor, DamageData, NewHitResult, OwnerCharacter);
			}
		}
	}

	FinishDamage(HitResult);
}

void AFPProjectileBase::FinishDamage(FHitResult HitResult)
{

	MeshComponent->SetVisibility(false);

	UFPGameplayFunctionLibrary::SpawnImpactEffect(ImpactEffectClass, HitResult);

	SetLifeSpan(0.1f);
}

void AFPProjectileBase::SetDamageData(FDamageData NewDamageData)
{
	DamageData = NewDamageData;
}
