// Fill out your copyright notice in the Description page of Project Settings.


#include "FPEnemy.h"


#include "FPHitMark.h"
#include "FPDamageNumber.h"
#include "FPAttributeComponent.h"
#include "FPWeaponBase.h"
#include "FPWeaponSystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"

static TAutoConsoleVariable<bool> CVarShowDamage(TEXT("fp.ToggleDamagePopUp"), true, TEXT("Taggole Whether to Show Damage PopUp or Not"));

AFPEnemy::AFPEnemy()
{
	AttributeComponent = CreateDefaultSubobject<UFPAttributeComponent>(TEXT("AttributeComponent"));
}

void AFPEnemy::BeginPlay()
{
	Super::BeginPlay();

	AttributeComponent->OnHealthChanged.AddDynamic(this, &AFPEnemy::OnHealthChanged);
}

bool AFPEnemy::TakeDamage_Implementation(FDamageData DamageData, FHitResult HitResult, AActor* InstigateActor)
{
	IFPDamageInterface::TakeDamage_Implementation(DamageData, HitResult, InstigateActor);

	return AttributeComponent->ApplyHealthChange(-DamageData.Damage, HitResult, InstigateActor);
}

void AFPEnemy::OnHealthChanged(AActor* InstigateActor, UFPAttributeComponent* OwnerComponent, float NewValue,
							   float Delta, FDamageResult DamageResult)
{
	if(OwnerComponent != AttributeComponent)
	{
		return;
	}

	if (Delta < 0.0f)
	{
		if (HitMark == nullptr)
		{
			HitMark = CreateWidget<UFPHitMark>(GetWorld(), HitMarkClass);
			if (HitMark)
			{
				HitMark->AddToViewport();
				HitMark->ActivateAnim(DamageResult.HitType);
			}
		}
		else
		{
			HitMark->ActivateAnim(DamageResult.HitType);
		}

		if (CVarShowDamage.GetValueOnGameThread())
		{
			UFPDamageNumber* DamagePopUp = CreateWidget<UFPDamageNumber>(GetWorld(), DamageNumberClass);
			if (DamagePopUp)
			{
				DamagePopUp->AddToViewport();
				DamagePopUp->Activate(Delta, DamageResult);
			}
		}

		if (NewValue == 0.0f)
		{
			// Set Ragdoll
			GetMesh()->SetSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			// Disable Collision
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}
