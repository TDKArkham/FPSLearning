// Fill out your copyright notice in the Description page of Project Settings.


#include "FPCharacter.h"

#include "WeaponBase.h"
#include "Camera/CameraComponent.h"


AFPCharacter::AFPCharacter()
{
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerArm"));
	SkeletalMeshComponent->SetupAttachment(Camera);
	SkeletalMeshComponent->SetCastShadow(false);
}

void AFPCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void AFPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
}

void AFPCharacter::MoveForward(float Axis)
{
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll = 0.0f;

	AddMovementInput(ControlRotation.Vector(), Axis);
}

void AFPCharacter::MoveRight(float Axis)
{
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll = 0.0f;
	FVector RightVector = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(RightVector, Axis);
}

void AFPCharacter::WeaponPickUp(TSubclassOf<AWeaponBase> WeaponToSpawn)
{
	WeaponSlots.Add(GetWorld()->SpawnActor<AWeaponBase>(WeaponToSpawn, GetActorTransform()));
	CurrentWeapon = WeaponSlots.Last();
	if (CurrentWeapon)
	{
		const FAttachmentTransformRules TransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, true);
		CurrentWeapon->AttachToComponent(SkeletalMeshComponent, TransformRules, CurrentWeapon->SocketName);

		EquipWeapon(CurrentWeapon);
	}
}

void AFPCharacter::EquipWeapon(AWeaponBase* Weapon)
{
	if (!bIsSwitchingWeapon)
	{
		if (bIsReloading)
		{
			bIsReloading = false;
		}

		if (CurrentWeapon != Weapon)
		{
			CurrentWeapon = Weapon;
		}

		bIsSwitchingWeapon = true;
		HideAndShowWeapon(Weapon);
		LoadOut = ELoadOut::ELO_HasWeapon;

		// TODO: Add a delay here to reset bIsSwitchingWeapon & bCanShoot state.
	}
}

void AFPCharacter::HideAndShowWeapon(AWeaponBase* Weapon)
{
	for (AWeaponBase* WeaponInSlot : WeaponSlots)
	{
		if (WeaponInSlot)
		{
			bool bShouldHide = (WeaponInSlot != Weapon);
			WeaponInSlot->SetActorHiddenInGame(bShouldHide);
		}
	}
}
