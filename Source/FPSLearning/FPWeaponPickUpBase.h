// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "FPWeaponPickUpBase.generated.h"

class USphereComponent;
class USoundCue;
class AFPWeaponBase;
class URotatingMovementComponent;
class UTimelineComponent;
class UCurveVector;

UCLASS()
class FPSLEARNING_API AFPWeaponPickUpBase : public AActor, public IFPGameplayInterface
{
	GENERATED_BODY()

public:
	AFPWeaponPickUpBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	URotatingMovementComponent* RotatingMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AFPWeaponBase> WeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	USoundCue* PickUpSound;

	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* TimelineComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	UCurveVector* MoveCurve;

	UFUNCTION()
	void OnBeginMovement(FVector Delta);

	UFUNCTION()
	void OnEndMovement(FVector Delta);

public:
	void Interact_Implementation(AActor* InstigateActor) override;
};
