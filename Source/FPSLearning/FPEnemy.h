// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPDamageInterface.h"
#include "GameFramework/Character.h"
#include "FPEnemy.generated.h"

class UFPAttributeComponent;

UCLASS()
class FPSLEARNING_API AFPEnemy : public ACharacter, public IFPDamageInterface
{
	GENERATED_BODY()

public:
	AFPEnemy();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UFPAttributeComponent* AttributeComponent;

public:
	virtual bool TakeDamage_Implementation(FDamageData DamageData, FHitResult HitResult, AActor* InstigateActor) override;
};
