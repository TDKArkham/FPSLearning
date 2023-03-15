// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPDamageInterface.h"
#include "GameFramework/Character.h"
#include "FPEnemy.generated.h"

class UFPAttributeComponent;
class UFPHitMark;
class UFPDamageNumber;

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

	UPROPERTY(BlueprintReadWrite, Category = "Widget")
	UFPHitMark* HitMark;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UFPHitMark> HitMarkClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UFPDamageNumber> DamageNumberClass;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigateActor, UFPAttributeComponent* OwnerComponent, float NewValue, float Delta, FDamageResult DamageResult);

public:
	virtual bool TakeDamage_Implementation(FDamageData DamageData, FHitResult HitResult, AActor* InstigateActor) override;
};
