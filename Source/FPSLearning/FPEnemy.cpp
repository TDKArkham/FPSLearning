// Fill out your copyright notice in the Description page of Project Settings.


#include "FPEnemy.h"

#include "FPAttributeComponent.h"


AFPEnemy::AFPEnemy()
{
	AttributeComponent = CreateDefaultSubobject<UFPAttributeComponent>(TEXT("AttributeComponent"));
}

void AFPEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

bool AFPEnemy::TakeDamage_Implementation(FDamageData DamageData, FHitResult HitResult, AActor* InstigateActor)
{
	IFPDamageInterface::TakeDamage_Implementation(DamageData, HitResult, InstigateActor);

	return AttributeComponent->ApplyHealthChange(-DamageData.Damage, HitResult, InstigateActor);
}
