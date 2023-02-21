// Fill out your copyright notice in the Description page of Project Settings.


#include "FPCharacter.h"

#include "FPWeaponBase.h"
#include "FPWeaponSystemComponent.h"
#include "Camera/CameraComponent.h"


AFPCharacter::AFPCharacter()
{
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerArm"));
	SkeletalMeshComponent->SetupAttachment(Camera);
	SkeletalMeshComponent->SetCastShadow(false);

	WeaponSystem = CreateDefaultSubobject<UFPWeaponSystemComponent>(TEXT("WeaponSystem"));
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

USkeletalMeshComponent* AFPCharacter::GetMeshComponent()
{
	return SkeletalMeshComponent;
}
