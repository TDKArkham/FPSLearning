#include "FPAmmoPickUpBase.h"

#include "FPWeaponBase.h"
#include "FPWeaponSystemComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


AFPAmmoPickUpBase::AFPAmmoPickUpBase()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AmmoMesh"));
	StaticMeshComponent->SetCollisionProfileName("NoCollision");
	RootComponent = StaticMeshComponent;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(RootComponent);
	SphereCollision->SetSphereRadius(13.0f);

	AmmoToGrant = 20;
}

void AFPAmmoPickUpBase::BeginPlay()
{
	Super::BeginPlay();
}

void AFPAmmoPickUpBase::Interact_Implementation(AActor* InstigateActor)
{
	IFPGameplayInterface::Interact_Implementation(InstigateActor);

	UFPWeaponSystemComponent* WeaponSystem = UFPWeaponSystemComponent::GetWeaponSystemComponent(InstigateActor);
	if (WeaponSystem)
	{
		if (WeaponSystem->AddAmmoToWeapon(AmmoType, AmmoToGrant))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), PickUpSound);
			Destroy();
		}
	}
}
