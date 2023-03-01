// Fill out your copyright notice in the Description page of Project Settings.


#include "FPWeaponBase.h"

#include "DrawDebugHelpers.h"
#include "FPCharacter.h"
#include "FPWeaponSystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"


AFPWeaponBase::AFPWeaponBase()
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
	SkeletalMeshComponent->SetCastShadow(false);

	/*ChamberAmmo = 1;*/
	MagazineAmmo = 30;
	TotalAmmo = 200;
	AmmoTypeText = FText::FromString("[Auto]");

	SocketName = "b_RightWeapon";

	BulletSpread = 35.0f;
	ShotRange = 10000.0f;
}

void AFPWeaponBase::BeginPlay()
{
	Super::BeginPlay();

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

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(Player);

		GetWorld()->LineTraceSingleByObjectType(HitResult, Start, End, ObjectParams, Params);

		/*FColor Color = HitResult.Actor.IsValid() ? FColor::Green : FColor::Red;
		DrawDebugLine(GetWorld(), Start, End, Color, false, 3.0f);*/

		return HitResult;
	}

	return HitResult;
}

void AFPWeaponBase::StartShooting(AFPCharacter* InstigateActor)
{
	switch (FireType)
	{
	case EFireType::EFT_HitScan:
		{
			CalculateLineTrace(InstigateActor);
			break;
		}
	case EFireType::EFT_Projectile:
		{
			break;
		}
	}
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
