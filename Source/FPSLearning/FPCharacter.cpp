// Fill out your copyright notice in the Description page of Project Settings.


#include "FPCharacter.h"

#include "FPWeaponBase.h"
#include "FPWeaponSystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AFPCharacter::AFPCharacter()
{
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerArm"));
	SkeletalMeshComponent->SetupAttachment(Camera);
	SkeletalMeshComponent->SetCastShadow(false);

	WeaponSystem = CreateDefaultSubobject<UFPWeaponSystemComponent>(TEXT("WeaponSystem"));

	SprintTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("SprintTimeline"));

	MaxWalkSpeed = 480.0f;
	MaxAimWalkSpeed = 250.0f;
	MaxSprintSpeed = 600.0f;

	EnergyLevel = 1.0f;
	StaminaRecoverDelay = 2.0f;
	ExhaustedRecoverDelay = 5.0f;

	FOVInterpSpeed = 7.0f;
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

void AFPCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	SetFieldOfView();
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

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPCharacter::Fire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AFPCharacter::StopFiring);

	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AFPCharacter::Aim);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &AFPCharacter::StopAiming);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AFPCharacter::Reload);
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

void AFPCharacter::Fire()
{
	if (bIsSprinting)
	{
		StopSprinting();
	}
	WeaponSystem->FireWeapon();
}

void AFPCharacter::StopFiring()
{
	WeaponSystem->StopShooting();
}

void AFPCharacter::Aim()
{
	WeaponSystem->Aim();
}

void AFPCharacter::StopAiming()
{
	WeaponSystem->StopAiming();
}

void AFPCharacter::Reload()
{
	WeaponSystem->ReloadWeapon();
}

void AFPCharacter::StartSprinting()
{
	if (!WeaponSystem->GetIsAiming())
	{
		if (!WeaponSystem->GetIsReloading())
		{
			if (!bIsExhausted && EnergyLevel >= 0.2f)
			{
				if (!GetCharacterMovement()->IsFalling() && GetCharacterMovement()->Velocity.Size() > 0.0f)
				{
					GetWorldTimerManager().ClearTimer(StaminaRecoverDelayHandle);

					SetPlayerSpeed(EMT_Sprint);
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
							WeaponSystem->StopShooting();
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
	if (!bIsExhausted)
	{
		SetPlayerSpeed(EMT_NormalWalk);
		bIsSprinting = false;

		SprintTimeline->Stop();

		GetWorldTimerManager().SetTimer(StaminaRecoverDelayHandle, this, &AFPCharacter::StaminaRecoverDelayFunc, StaminaRecoverDelay);
	}
}

void AFPCharacter::Exhausted()
{
	bIsExhausted = true;
	SetPlayerSpeed(EMT_NormalWalk);
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
	if (EnergyLevel == 0.0f)
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

float AFPCharacter::PlayAnimMontageOnArm(UAnimMontage* AnimMontage, float InPlayRate)
{
	UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();
	if (AnimMontage && AnimInstance)
	{
		float Duration = AnimInstance->Montage_Play(AnimMontage, InPlayRate);

		return Duration;
	}

	return 0.0f;
}

void AFPCharacter::SetPlayerSpeed(EMoveType MoveType)
{
	float TargetSpeed = MaxWalkSpeed;

	switch (MoveType)
	{
	case EMT_NormalWalk:
		{
			TargetSpeed = MaxWalkSpeed;
			break;
		}
	case EMT_AimWalk:
		{
			TargetSpeed = MaxAimWalkSpeed;
			break;
		}
	case EMT_Sprint:
		{
			TargetSpeed = MaxSprintSpeed;
			break;
		}
	}

	GetCharacterMovement()->MaxWalkSpeed = TargetSpeed;
}

void AFPCharacter::SetFieldOfView()
{
	if(WeaponSystem->GetCurrentWeapon())
	{
		float TargetFieldOfView = WeaponSystem->GetIsAiming()
			? FMath::FInterpTo(GetCamera()->FieldOfView,
							   WeaponSystem->GetCurrentWeapon()->AimFieldOfView,
							   UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), FOVInterpSpeed)
			: FMath::FInterpTo(GetCamera()->FieldOfView,
							   WeaponSystem->GetCurrentWeapon()->HipFieldOfView,
							   UGameplayStatics::GetWorldDeltaSeconds(GetWorld()),
							   FOVInterpSpeed);
		GetCamera()->SetFieldOfView(TargetFieldOfView);
	}
}

UCameraComponent* AFPCharacter::GetCamera()
{
	return Camera;
}

USkeletalMeshComponent* AFPCharacter::GetMeshComponent()
{
	return SkeletalMeshComponent;
}

UFPMainHUD* AFPCharacter::GetMainHUD()
{
	return MainHUD;
}

bool AFPCharacter::GetIsSprinting()
{
	return bIsSprinting;
}
