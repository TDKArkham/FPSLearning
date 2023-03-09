// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FPAttributeComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FPSLEARNING_API UFPAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFPAttributeComponent();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float MaxHealth;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float CurrentHealth;

	UFUNCTION(BlueprintCallable, Category = "Health")
	bool ApplyHealthChange(float Delta, FHitResult HitResult, AActor* InstigateActor);

	UFUNCTION(BlueprintCallable, Category = "Health")
	bool GetIsAlive() const;
};
