// Fill out your copyright notice in the Description page of Project Settings.


#include "FPWeaponBase.h"

#include "DrawDebugHelpers.h"
#include "FPAttributeComponent.h"
#include "FPCharacter.h"
#include "FPDamageInterface.h"
#include "FPGameplayFunctionLibrary.h"
#include "FPImpactEffectBase.h"
#include "FPProjectileBase.h"
#include "FPWeaponSystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

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
	AmmoDifference = 0;
	AmmoTypeText = FText::FromString("[Auto]");

	SocketName = "b_RightWeapon";

	bIsShooting = false;
	bIsFireIntervalActive = false;

	BulletSpread = 35.0f;
	AimBulletSpread = 20.0f;
	ShotRange = 10000.0f;
	PelletCount = 0;
	BulletPerMin = 0;

	AimFieldOfView = 70.0f;
	HipFieldOfView = 90.0f;

	BlendInTime = 0.1f;
	BlendOutTime = 0.1f;

	DamageData.Damage = 20.0f;
	DamageData.ImpactRadius = 50.0f;

	VerticalRecoil = -0.05f;
	HorizontalRecoil = 0.15f;

	ImpulseStrength = 10000.0f;
}

void AFPWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmo = MagazineAmmo;

	OwnerCharacter = Cast<AFPCharacter>(GetOwner());
	OwnerComponent = UFPWeaponSystemComponent::GetWeaponSystemComponent(OwnerCharacter);
	if (OwnerComponent)
	{
		OwnerComponent->OnAimEnter.AddDynamic(this, &AFPWeaponBase::OnWeaponAimingEnter);
		OwnerComponent->OnAimExit.AddDynamic(this, &AFPWeaponBase::OnWeaponAimingExit);
	}

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

FHitResult AFPWeaponBase::CalculateLineTrace()
{
	FHitResult HitResult;
	if (OwnerCharacter)
	{
		FVector Start = OwnerCharacter->GetCamera()->GetComponentLocation();
		FVector End = OwnerCharacter->GetCamera()->GetComponentRotation().Vector() * ShotRange + Start;
		float TargetSpread = 0.0f;
		if (OwnerComponent)
		{
			if (OwnerComponent->GetIsAiming())
			{
				TargetSpread = WeaponType == EWeaponType::EWT_ShotGun ? AimBulletSpread : 0.0f;
			}
			else
			{
				TargetSpread = BulletSpread;
			}
		}
		End.X += FMath::FRandRange(-TargetSpread, TargetSpread);
		End.Y += FMath::FRandRange(-TargetSpread, TargetSpread);
		End.Z += FMath::FRandRange(-TargetSpread, TargetSpread);

		FCollisionObjectQueryParams ObjectParams;
		ObjectParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectParams.AddObjectTypesToQuery(ECC_PhysicsBody);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(OwnerCharacter);
		Params.bReturnPhysicalMaterial = true;

		GetWorld()->LineTraceSingleByObjectType(HitResult, Start, End, ObjectParams, Params);

		// DEBUG: Remove this code later.
		FColor Color = HitResult.Actor.IsValid() ? FColor::Green : FColor::Red;
		DrawDebugLine(GetWorld(), Start, End, Color, false, 3.0f);

		return HitResult;
	}

	return HitResult;
}

FTransform AFPWeaponBase::CalculateProjectile(FName WeaponSocketName)
{
	FVector SpawnLocation = SkeletalMeshComponent->GetSocketLocation(WeaponSocketName);

	FTransform ProjectileTransform;
	ProjectileTransform.SetTranslation(SpawnLocation);

	if (WeaponType == EWeaponType::EWT_SniperRifle)
	{
		FHitResult TargetFindResult = CalculateLineTrace();
		FRotator BulletSpeedDirection = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, TargetFindResult.ImpactPoint);
		ProjectileTransform.SetRotation(BulletSpeedDirection.Quaternion());
	}
	else
	{
		if (OwnerCharacter)
		{
			ProjectileTransform.SetRotation(OwnerCharacter->GetControlRotation().Quaternion());
		}
	}

	return ProjectileTransform;
}

void AFPWeaponBase::SpawnProjectile(FTransform SpawnTransform)
{
	if (ensure(ProjectileClass))
	{
		AFPProjectileBase* Projectile = Cast<AFPProjectileBase>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), ProjectileClass, SpawnTransform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
		Projectile->SetDamageData(DamageData);
		Projectile->SetOwner(this);
		Projectile->SetInstigator(OwnerCharacter);
		UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);
	}
}

void AFPWeaponBase::ApplyDamageOnHitScan(FHitResult HitResult)
{
	AActor* HitActor = HitResult.GetActor();
	if (HitActor && HitActor->Implements<UFPDamageInterface>())
	{
		IFPDamageInterface::Execute_TakeDamage(HitActor, DamageData, HitResult, OwnerCharacter);
	}
}

void AFPWeaponBase::StartShooting_Implementation()
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

	}

	switch (FireType)
	{
	case EFireType::EFT_HitScan:
		{
			if (WeaponType == EWeaponType::EWT_ShotGun)
			{
				for (int32 Index = 0; Index < PelletCount; Index++)
				{
					FHitResult HitResult = CalculateLineTrace();
					UFPGameplayFunctionLibrary::SpawnImpactEffect(ImpactEffectClass, HitResult);
					ApplyDamageOnHitScan(HitResult);
				}
			}
			else
			{
				FHitResult HitResult = CalculateLineTrace();
				UFPGameplayFunctionLibrary::SpawnImpactEffect(ImpactEffectClass, HitResult);
				ApplyDamageOnHitScan(HitResult);
			}
			break;
		}
	case EFireType::EFT_Projectile:
		{
			// TODO: Promote the socket name to a variable later when changing the weapon pack.
			SpawnProjectile(CalculateProjectile("MuzzleFlash"));
			break;
		}
	}

	StartRecoil();
	bIsShooting = true;
	if (OwnerComponent)
	{
		OwnerComponent->OnAmmoChanged.Broadcast(CurrentAmmo, TotalAmmo, AmmoTypeText);
	}
}

void AFPWeaponBase::StopShooting_Implementation()
{
	bIsShooting = false;
}

void AFPWeaponBase::OnWeaponAimingEnter_Implementation(AActor* CurrentWeapon)
{
	bIsWeaponAiming = true;
}

void AFPWeaponBase::OnWeaponAimingExit_Implementation(AActor* CurrentWeapon)
{
	bIsWeaponAiming = false;
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
