// Fill out your copyright notice in the Description page of Project Settings.


#include "FPWeaponBase.h"

#include "DrawDebugHelpers.h"
#include "FPCharacter.h"
#include "FPDamageInterface.h"
#include "FPImpactEffectBase.h"
#include "FPWeaponSystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"


AFPWeaponBase::AFPWeaponBase()
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
	SkeletalMeshComponent->SetCastShadow(false);

	RecoilTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("RecoilTimeline"));
	RecoilTimeline->SetLooping(false);

	/*ChamberAmmo = 1;*/
	MagazineAmmo = 30;
	TotalAmmo = 200;
	AmmoTypeText = FText::FromString("[Auto]");

	SocketName = "b_RightWeapon";

	BulletSpread = 35.0f;
	ShotRange = 10000.0f;
	PelletCount = 0;
	BulletPerMin = 0;

	DamageData.Damage = 20.0f;
	DamageData.ImpactRadius = 50.0f;

	VerticalRecoil = -0.05f;
	HorizontalRecoil = 0.15f;
}

void AFPWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmo = MagazineAmmo;

	if (RecoilCurve)
	{
		FOnTimelineFloat StartTimelineFloat;
		StartTimelineFloat.BindUFunction(this, "UpdatingTimeline");
		RecoilTimeline->AddInterpFloat(RecoilCurve, StartTimelineFloat);

		FOnTimelineEventStatic EndTimelineFloat;
		EndTimelineFloat.BindUFunction(this, "TimelineFinished");
		RecoilTimeline->SetTimelineFinishedFunc(EndTimelineFloat);

		RecoilTimeline->SetTimelineLengthMode(TL_LastKeyFrame);
	}
}

FHitResult AFPWeaponBase::CalculateLineTrace(AFPCharacter* Player)
{
	FHitResult HitResult;
	if (Player)
	{
		UFPWeaponSystemComponent* WeaponSystem = UFPWeaponSystemComponent::GetWeaponSystemComponent(Player);

		FVector Start = Player->GetCamera()->GetComponentLocation();
		FVector End = Player->GetCamera()->GetComponentRotation().Vector() * ShotRange + Start;
		if (WeaponSystem && !WeaponSystem->GetIsAiming())
		{
			End.X += FMath::FRandRange(-BulletSpread, BulletSpread);
			End.Y += FMath::FRandRange(-BulletSpread, BulletSpread);
			End.Z += FMath::FRandRange(-BulletSpread, BulletSpread);
		}

		FCollisionObjectQueryParams ObjectParams;
		ObjectParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectParams.AddObjectTypesToQuery(ECC_PhysicsBody);
		

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(Player);
		Params.bReturnPhysicalMaterial = true;

		// DEBUG: Remove this code later.
		GetWorld()->LineTraceSingleByObjectType(HitResult, Start, End, ObjectParams, Params);

		FColor Color = HitResult.Actor.IsValid() ? FColor::Green : FColor::Red;
		DrawDebugLine(GetWorld(), Start, End, Color, false, 3.0f);

		return HitResult;
	}

	return HitResult;
}

void AFPWeaponBase::SpawnImpactEffect(FHitResult HitResult)
{
	if (ensure(ImpactEffectClass))
	{
		const FTransform SpawnTransform(HitResult.ImpactPoint);
		AFPImpactEffectBase* ImpactEffect = Cast<AFPImpactEffectBase>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), ImpactEffectClass, SpawnTransform));
		ImpactEffect->HitResult = HitResult;
		UGameplayStatics::FinishSpawningActor(ImpactEffect, SpawnTransform);
	}
}

void AFPWeaponBase::ApplyDamageOnHitScan(FHitResult HitResult)
{
	AActor* HitActor = HitResult.GetActor();
	if (HitActor && HitActor->Implements<UFPDamageInterface>())
	{
		IFPDamageInterface::Execute_TakeDamage(HitActor, DamageData, HitResult, this);
	}
}

void AFPWeaponBase::StartShooting_Implementation(AFPCharacter* InstigateActor, UFPWeaponSystemComponent* InstigateWeaponSystem)
{
	if (CurrentAmmo <= 0)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), DryFireSound);
		return;
	}

	CurrentAmmo -= 1;

	if (FireAnim)
	{
		SkeletalMeshComponent->PlayAnimation(FireAnim, false);
	}

	if (CurrentAmmo <= 0)
	{
		// Only auto reload when using Grenade Launcher or Rocket Launcher.
		if (WeaponType == EWeaponType::EWT_GrenadeLauncher || WeaponType == EWeaponType::EWT_GrenadeLauncher)
		{
			AutoReloadOnEmpty(InstigateActor);
		}
	}

	switch (FireType)
	{
		case EFireType::EFT_HitScan:
			{
				if (WeaponType == EWeaponType::EWT_ShotGun)
				{
					for (int32 Index = 0; Index < PelletCount; Index++)
					{
						FHitResult HitResult = CalculateLineTrace(InstigateActor);
						SpawnImpactEffect(HitResult);
						ApplyDamageOnHitScan(HitResult);
					}
				}
				else
				{
					FHitResult HitResult = CalculateLineTrace(InstigateActor);
					SpawnImpactEffect(HitResult);
					ApplyDamageOnHitScan(HitResult);
				}
				break;
			}
		case EFireType::EFT_Projectile:
			{
				break;
			}
	}

	StartRecoil();
	bIsShooting = true;
	if (InstigateWeaponSystem)
	{
		InstigateWeaponSystem->OnAmmoChanged.Broadcast(CurrentAmmo, TotalAmmo, AmmoTypeText);
	}
}

void AFPWeaponBase::StopShooting_Implementation(/*AFPCharacter* InstigateActor*/)
{
	bIsShooting = false;
}

void AFPWeaponBase::StartRecoil()
{
	RecoilTimeline->PlayFromStart();
}

void AFPWeaponBase::StopRecoil()
{
	RecoilTimeline->Stop();
}

void AFPWeaponBase::ReverseRecoil()
{
	RecoilTimeline->Reverse();
}

void AFPWeaponBase::BeginFireInterval()
{
	bIsFireIntervalActive = true;

	FTimerHandle FireIntervalHandle;
	if (!GetWorldTimerManager().IsTimerActive(FireIntervalHandle))
	{
		GetWorldTimerManager().SetTimer(FireIntervalHandle, this, &AFPWeaponBase::ResetFireInterval, 60.0f / BulletPerMin);
	}
}

void AFPWeaponBase::ResetFireInterval()
{
	bIsFireIntervalActive = false;
}

void AFPWeaponBase::UpdatingTimeline(float Alpha)
{
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (Player)
	{
		float PitchDelta = FMath::Lerp(0.0f, VerticalRecoil, Alpha);
		bool bIsReversing = RecoilTimeline->IsReversing();

		PitchDelta = !bIsReversing ? PitchDelta : -PitchDelta;

		Player->AddControllerPitchInput(PitchDelta);

		if (!bIsReversing)
		{
			Player->AddControllerYawInput(FMath::FRandRange(HorizontalRecoil, -HorizontalRecoil));
		}
	}

}

void AFPWeaponBase::TimelineFinished()
{
	ReverseRecoil();
	FTimerHandle StopRecoilDelayHandle;
	GetWorldTimerManager().SetTimer(StopRecoilDelayHandle, this, &AFPWeaponBase::TimelineFinished_TimeElapsed, 0.08f);
}

void AFPWeaponBase::TimelineFinished_TimeElapsed()
{
	StopRecoil();
}

bool AFPWeaponBase::AddTotalAmmo(EAmmoType AcquiredAmmoType, int32 AcquiredAmmo)
{
	if (AcquiredAmmoType == AmmoType)
	{
		TotalAmmo += AcquiredAmmo;
		return true;
	}
	return false;
}

void AFPWeaponBase::AutoReloadOnEmpty(AFPCharacter* InstigateActor)
{
	/*if (WeaponType == EWeaponType::EWT_SniperRifle)
	{
		FTimerHandle AutoReloadHandle;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "AutoReloadOnEmpty_TimeElapsed", InstigateActor);
		GetWorld()->GetTimerManager().SetTimer(AutoReloadHandle, Delegate, 0.3f, false);
	}
	else
	{
		InstigateActor->Reload();
	}*/

	InstigateActor->Reload();
}

void AFPWeaponBase::AutoReloadOnEmpty_TimeElapsed(AFPCharacter* InstigateActor)
{
	InstigateActor->Reload();
}

void AFPWeaponBase::ReloadCalculate()
{
	AmmoDifference = MagazineAmmo - CurrentAmmo;
	if (AmmoDifference <= TotalAmmo)
	{
		CurrentAmmo = MagazineAmmo;
		TotalAmmo -= AmmoDifference;
	}
	else
	{
		CurrentAmmo += TotalAmmo;
		TotalAmmo = 0;
	}
}

float AFPWeaponBase::PlayAnimMontageOnWeapon(float InPlayRate)
{
	UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();
	if (WeaponMontage && AnimInstance)
	{
		float Duration = AnimInstance->Montage_Play(WeaponMontage, InPlayRate);

		return Duration;
	}

	return 0.0f;
}

bool AFPWeaponBase::IsMagazineFull()
{
	return CurrentAmmo == MagazineAmmo;
}

bool AFPWeaponBase::HasLoadedAmmo()
{
	return CurrentAmmo > 0;
}

bool AFPWeaponBase::HasReservedAmmo()
{
	return TotalAmmo > 0;
}
