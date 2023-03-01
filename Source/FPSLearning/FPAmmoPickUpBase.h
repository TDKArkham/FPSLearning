#pragma once

#include "CoreMinimal.h"
#include "FPGameplayInterface.h"
#include "WeaponData.h"
#include "GameFramework/Actor.h"
#include "FPAmmoPickUpBase.generated.h"

class USphereComponent;

UCLASS()
class FPSLEARNING_API AFPAmmoPickUpBase : public AActor, public IFPGameplayInterface
{
	GENERATED_BODY()
	
public:
	AFPAmmoPickUpBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Ammo")
	EAmmoType AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Ammo")
	int32 AmmoToGrant;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Ammo")
	USoundBase* PickUpSound;

public:
	void Interact_Implementation(AActor* InstigateActor) override;
};
