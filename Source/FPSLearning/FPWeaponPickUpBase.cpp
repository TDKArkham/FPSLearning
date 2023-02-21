// Fill out your copyright notice in the Description page of Project Settings.


#include "FPWeaponPickUpBase.h"

#include "FPWeaponBase.h"

#include "FPWeaponSystemComponent.h"
#include "Components/SphereComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


// Sets default values
AFPWeaponPickUpBase::AFPWeaponPickUpBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = SkeletalMeshComponent;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(RootComponent);

	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovement"));
	RotatingMovement->RotationRate.Yaw = 130;

	TimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("UpAndDownTimeline"));
	TimelineComponent->SetLooping(true);
}

// Called when the game starts or when spawned
void AFPWeaponPickUpBase::BeginPlay()
{
	Super::BeginPlay();

	if (MoveCurve)
	{
		FOnTimelineVector StartTimelineVector;
		StartTimelineVector.BindUFunction(this, "OnBeginMovement");
		TimelineComponent->AddInterpVector(MoveCurve, StartTimelineVector);

		FOnTimelineEventStatic EndTimelineVector;
		EndTimelineVector.BindUFunction(this, "OnEndMovement");
		TimelineComponent->SetTimelineFinishedFunc(EndTimelineVector);

		TimelineComponent->SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);
		
		TimelineComponent->PlayFromStart();
	}
}

void AFPWeaponPickUpBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	/*if(TimelineComponent->IsPlaying())
	{
		TimelineComponent.tick
	}*/
}

void AFPWeaponPickUpBase::Interact_Implementation(AActor* InstigateActor)
{
	IFPGameplayInterface::Interact_Implementation(InstigateActor);

	UFPWeaponSystemComponent* WeaponSystem = UFPWeaponSystemComponent::GetWeaponSystemComponent(InstigateActor);
	if (WeaponSystem)
	{
		WeaponSystem->WeaponPickUp(WeaponClass);
		UGameplayStatics::PlaySound2D(GetWorld(), PickUpSound);
		Destroy();
	}
}

void AFPWeaponPickUpBase::OnBeginMovement(FVector Delta)
{
	AddActorLocalOffset(Delta);
}

void AFPWeaponPickUpBase::OnEndMovement(FVector Delta)
{
	
}
