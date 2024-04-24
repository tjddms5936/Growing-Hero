// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitAnimInstance.h"
#include "UnitBase.h"
#include "CombatComponent.h"
#include "Kismet/KismetMathLibrary.h"
// #include "AnimGraphRuntime//Public/KismetAnimationLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

void UUnitAnimInstance::NativeInitializeAnimation()
{
	// BeginPlay()와 같은 역할
	if (Pawn == nullptr) {
		Pawn = TryGetPawnOwner();
		if (Pawn) {
			Unit = Cast<AUnitBase>(Pawn);
			m_pController = Cast<AMyCharacterController>(Unit->GetController());
		}
	}
	else {
		// Pawn 존재함
		Unit = Cast<AUnitBase>(Pawn);
		m_pController = Cast<AMyCharacterController>(Unit->GetController());
	}
}

void UUnitAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (Pawn == nullptr) {
		Pawn = TryGetPawnOwner();
		if (Pawn) {
			Unit = Cast<AUnitBase>(Pawn);
			m_pController = Cast<AMyCharacterController>(Unit->GetController());
		}
	}

	if (Pawn) {
		FVector Speed = Pawn->GetVelocity(); // 특정 프레임에서 어떤 스피드를 가지고 있는지 반환함
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		MovementSpeed = LateralSpeed.Size();
		MovementDirection = CalculateDirection(Pawn->GetVelocity(), Pawn->GetActorRotation());

		// 점프 관련 변수 구하기
		IsInAir = Unit->GetMovementComponent()->IsFalling();
		m_ZVelocity = UKismetMathLibrary::FInterpTo(m_ZVelocity, Speed.Z, DeltaTime, 5.f);

		if (!Unit->IsAdventureMode())
		{
			AimPitch = 0.f;
			AimYaw = 0.f;
			return;
		}

		// 방향 구하기
		FRotator TmpRotator = UKismetMathLibrary::RInterpTo(UKismetMathLibrary::MakeRotator(0.0f, AimPitch, AimYaw),
			UKismetMathLibrary::NormalizedDeltaRotator(Pawn->GetControlRotation(), Pawn->GetActorRotation()),
			DeltaTime, 15.f);

		AimPitch = UKismetMathLibrary::ClampAngle(TmpRotator.Pitch, -90.f, 90.f);
		AimYaw = UKismetMathLibrary::ClampAngle(TmpRotator.Yaw, -90.f, 90.f);
		if (abs(AimYaw) > 50.f && IsValid(Unit))
		{
			Unit->GetCharacterMovement()->bUseControllerDesiredRotation = true;
		}
		else if(abs(AimYaw) < 10.f && IsValid(Unit))
		{
			Unit->GetCharacterMovement()->bUseControllerDesiredRotation = false;
		}
	}
}
