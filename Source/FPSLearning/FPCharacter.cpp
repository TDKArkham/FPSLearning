// Fill out your copyright notice in the Description page of Project Settings.


#include "FPCharacter.h"

#include "FPWeaponBase.h"
#include "FPWeaponSystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


AFPCharacter::AFPCharacter()
{
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerArm"));
	SkeletalMeshComponent->SetupAttachment(Camera);
	SkeletalMeshComponent->SetCastShadow(false);

	WeaponSystem = CreateDefaultSubobject<UFPWeaponSystemComponent>(TEXT("WeaponSystem"));

	SprintTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("SprintTimeline"));

	EnergyLevel = 1.0f;
	MaxSprintSpeed = 600.0f;
	MaxWalkSpeed = 400.0f;
	StaminaRecoverDelay = 2.0f;
	ExhaustedRecoverDelay = 5.0f;
}

void AFPCharacter::BeginPlay()
{
	Super::BeginPlay();

	// TODO: Figure out why the hell this doesn't work.
	// Now I understand that you have to pass a PlayerController instead of a "this" Character
	// into the first parameter, if I use GetPlayerController() func or just set it to nullptr
	// , this would work.
	/*MainHUD = CreateWidget<UFPMainHUD>(this, UFPMainHUD::StaticClass());
	if (MainHUD)
	{
		MainHUD->AddToViewport();
	}*/
	if (SprintCurve)
	{
		FOnTimelineFloat StartTimelineFloat;
		StartTimelineFloat.BindUFunction(this, "UpdatingTimeline");
		SprintTimeline->AddInterpFloat(SprintCurve, StartTimelineFloat);

		FOnTimelineEventStatic EndTimelineFloat;
		EndTimelineFloat.BindUFunction(this, "TimelineFinished");
		SprintTimeline->SetTimelineFinishedFunc(EndTimelineFloat);

		SprintTimeline->SetTimelineLengthMode(TL_LastKeyFrame);
	}
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

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFPCharacter::StartSprinting);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AFPCharacter::StopSprinting);
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

void AFPCharacter::StartSprinting()
{
	if (!WeaponSystem->GetIsAiming())
	{
		if (!WeaponSystem->GetIsReloading())
		{
			if (!bIsExhausted && EnergyLevel >= 0.2f)
			{
				if (!GetCharacterMovement()->IsFalling())
				{
					GetWorldTimerManager().ClearTimer(StaminaRecoverDelayHandle);
					
					GetCharacterMovement()->MaxWalkSpeed = MaxSprintSpeed;
					bIsSprinting = true;

					switch (WeaponSystem->LoadOut)
					{
					case ELoadOut::ELO_NoWeapon:
					{
						SprintTimeline->Play();
						break;
					}
					case ELoadOut::ELO_HasWeapon:
					{
						SprintTimeline->Play();
						break;
					}
					default:;
					}
				}
			}
		}
	}
}

void AFPCharacter::StopSprinting()
{
	if(!bIsExhausted)
	{
		GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
		bIsSprinting = false;

		SprintTimeline->Stop();
		
		GetWorldTimerManager().SetTimer(StaminaRecoverDelayHandle, this, &AFPCharacter::StaminaRecoverDelayFunc, StaminaRecoverDelay);
	}
}

void AFPCharacter::Exhausted()
{
	bIsExhausted = true;
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	bIsSprinting = false;

	FTimerHandle ExhaustedRecoverDelayHandle;
	GetWorldTimerManager().SetTimer(ExhaustedRecoverDelayHandle, this, &AFPCharacter::ExhaustedRecoverDelayFunc, ExhaustedRecoverDelay);
}

void AFPCharacter::UpdatingTimeline(float Value)
{
	EnergyLevel = Value;
}

void AFPCharacter::TimelineFinished(float Value)
{
	// TimelineFinishedFunc is ONLY called when play to the end or revers to the beginning.
	// We use this check to make sure we won't call Exhausted() when it reverses to the beginning
	// which happens after stamina recovered
	if(EnergyLevel == 0.0f)
	{
		Exhausted();
	}
}

void AFPCharacter::StaminaRecoverDelayFunc()
{
	SprintTimeline->Reverse();
}

void AFPCharacter::ExhaustedRecoverDelayFunc()
{
	bIsExhausted = false;
	SprintTimeline->ReverseFromEnd();
}

USkeletalMeshComponent* AFPCharacter::GetMeshComponent()
{
	return SkeletalMeshComponent;
}

UFPMainHUD* AFPCharacter::GetMainHUD()
{
	return MainHUD;
}
