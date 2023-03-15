// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponData.h"
#include "Components/ActorComponent.h"
#include "FPAttributeComponent.generated.h"

class UFPAttributeComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnAttributeChanged, AActor*, InstigateActor, UFPAttributeComponent*, OwnerComponent, float, NewValue, float, Delta, FDamageResult, DamageResult);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FPSLEARNING_API UFPAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFPAttributeComponent();

	virtual void BeginPlay() override;
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float MaxHealth;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float CurrentHealth;

	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnAttributeChanged OnHealthChanged;

	UFUNCTION(BlueprintCallable, Category = "Health")
	bool ApplyHealthChange(float Delta, FHitResult HitResult, AActor* InstigateActor);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health")
	bool GetIsAlive() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health")
	static bool GetIsActorAlive(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health")
	static UFPAttributeComponent* GetAttributeComponent(AActor* TargetActor);
};
