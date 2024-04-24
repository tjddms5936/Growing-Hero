// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "UnitBase.h"
#include "MyCharacterController.h"

#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent() :
	m_eTargetType{},
	m_bIsComboOn{},
	m_ComboSectionName{},
	m_MaxAttackKind{},
	m_bHitBackOn{}
{
	PrimaryComponentTick.bCanEverTick = true;
	m_MaxAttackKind = 2;
	m_bHitBackOn = false;
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	Owner = Cast<AUnitBase>(GetOwner());
	if (!ensure(Owner != nullptr))
		return;

	clearTarget();
	AnimInstance = Owner->GetMesh()->GetAnimInstance();

}

void UCombatComponent::RotateToTarget(float DeltaTime)
{
	// 위 두가지 조건이 만족하면 보간 시작 : 내가 적을 바라보는 보간 
	if (!IsValid(Owner) || Owner->GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Flying)
		return;

	if (IsValid(m_pTarget) && m_pTarget->getUnitState() != EUNIT_STATE::E_Dead)
	{
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), m_pTarget->GetActorLocation());
		FRotator LookAtRotationYaw(0.f, LookAtRotation.Yaw, 0.f);
		FRotator InterpRotation = FMath::RInterpTo(GetOwner()->GetActorRotation(), LookAtRotationYaw, DeltaTime, 8.f);

		Owner->SetActorRotation(InterpRotation);
	}
}


// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (Owner->getUnitState() == EUNIT_STATE::E_Attack)
	{
		RotateToTarget(DeltaTime);
	}
}

void UCombatComponent::TakeDamage(float Damage)
{
	if (Owner->getUnitState() == EUNIT_STATE::E_Dead || Damage <= 0)
	{
		return;
	}
	Damage -= Owner->getUnitStat()->DEF;
	if (Damage <= 0)
		Damage = 1;
	Owner->getUnitStat()->m_HP -= Damage;
	Owner->TakeDamageEvent(Damage);
	if (Owner->getUnitStat()->m_HP <= 0)
	{
		Owner->getUnitStat()->m_HP = 0;
		Owner->DieEvent();
		Die();
		return;
	}

	if (Owner->getUnitState() == EUNIT_STATE::E_Attack || Owner->getUnitState() == EUNIT_STATE::E_UnderAttack)
		return;

	if (m_bHitBackOn)
	{
		// hit back Logic
		Owner->GetController()->StopMovement();
		if (AnimInstance && CombatMontage)
		{
			Owner->setUnitState(EUNIT_STATE::E_UnderAttack);
			AnimInstance->Montage_Play(CombatMontage, 1.f); // 0.5로하면 기존 스피드 1/2배
			AnimInstance->Montage_JumpToSection(FName("HitBack"), CombatMontage);
		}
		return;
	}

	int32 Random = FMath::RandRange(0, 1);
	if (Random)
	{
		Owner->GetController()->StopMovement();
		if (AnimInstance && CombatMontage)
		{
			Owner->setUnitState(EUNIT_STATE::E_UnderAttack);
			AnimInstance->Montage_Play(CombatMontage, 1.f); // 0.5로하면 기존 스피드 1/2배
			AnimInstance->Montage_JumpToSection(FName("TakeDamage"), CombatMontage);
		}
	}
}

void UCombatComponent::TakeDamageEnd()
{
	if (Owner->getUnitState() != EUNIT_STATE::E_Dead)
	{
		Owner->setUnitState(EUNIT_STATE::E_Idle);
	}
}

void UCombatComponent::ApplyDamage()
{
	if (m_pTarget == nullptr || DamageTypeClass == nullptr || m_pTarget->getUnitState() == EUNIT_STATE::E_Dead)
		return;


	TSubclassOf<UDamageType> const ValidDamageTypeClass = DamageTypeClass ? DamageTypeClass : TSubclassOf<UDamageType>(UDamageType::StaticClass());
	
	FDamageEvent DamageEvent(ValidDamageTypeClass);

	int32 nTotalDmg{};
	nTotalDmg = Owner->getUnitStat()->ATK + FMath::RandRange(Owner->getUnitStat()->STR * 5, Owner->getUnitStat()->STR * 10);
	
	UE_LOG(LogTemp, Warning, TEXT("ApplyDamage OK"));
	m_pTarget->TakeDamage(nTotalDmg, DamageEvent, Owner->GetController(), Owner);
}

void UCombatComponent::Die()
{
	// 죽음 애니매이션 시작. 
	// 무기 스켈레톤있으면 피직스 On & 콜리전프리셋(OverlapAll) 설정
	// 모든 움직임 박탈. 안그러면 넘어졌다가 원상복귀해버림 사라지기 전에. 즉, 일어나서 사라짐

	Owner->setUnitState(EUNIT_STATE::E_Dead);
	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage, 1.f); // 0.5로하면 기존 스피드 1/2배
		AnimInstance->Montage_JumpToSection(FName("Die"), CombatMontage);
	}
}

void UCombatComponent::DieEnd()
{
	// 액터 Destroy작업

	Owner->GetMesh()->bPauseAnims = true;
	Owner->GetMesh()->bNoSkeletonUpdate = true;
	Owner->DestroyChar();
}

void UCombatComponent::ReleaseCollision()
{
	Owner->GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Ignore);
	Owner->GetMesh()->SetSimulatePhysics(true);
}

void UCombatComponent::Attack()
{
	if (m_pTarget == nullptr || (m_pTarget != nullptr && m_pTarget->getUnitType() != m_eTargetType))
	{
		return;
	}

	if (m_pTarget->getUnitState() == EUNIT_STATE::E_Dead ||
		Owner->getUnitState() == EUNIT_STATE::E_UnderAttack)
	{
		return;
	}

	if (Owner->getUnitState() == EUNIT_STATE::E_Attack)
	{
		if(CanCombo())
			comboActive();
		return;
	}

	Owner->setUnitState(EUNIT_STATE::E_Attack);
	// 공격 몽타주 재생. 그리고 애니메이션 타이밍 맞춰서 ApplyDamage() 호출하면 될듯?
	AnimInstance = Owner->GetMesh()->GetAnimInstance();
	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage, Owner->getUnitStat()->AttackSpeed); // 0.5로하면 기존 스피드 1/2배
		int32 Section = FMath::RandRange(1, m_MaxAttackKind);
		switch (Section)
		{
		case 1:
			AnimInstance->Montage_JumpToSection(FName("Attack1"), CombatMontage);
			break;
		case 2:
			AnimInstance->Montage_JumpToSection(FName("Attack2"), CombatMontage);
			break;
		case 3:
			AnimInstance->Montage_JumpToSection(FName("Attack3"), CombatMontage);
			break;
		case 4:;
			// AnimInstance->Montage_JumpToSection(FName("m_MaxAttackKind Default"), CombatMontage);
			break;
		default:
			break;
		}
	}

}

void UCombatComponent::AttackEnd()
{
	// 공격 애니메이션에서 동작의 끝 지점에 호출되는 함수.
	Owner->setUnitState(EUNIT_STATE::E_Idle);
}

void UCombatComponent::setComboOn(bool IsOn)
{
	m_bIsComboOn = IsOn;
}

void UCombatComponent::setNextComboSectionName(FName NextSectionName)
{
	m_ComboSectionName = NextSectionName;
}

AUnitBase* UCombatComponent::getTarget()
{
	return m_pTarget;
}

void UCombatComponent::setTargetClear()
{
	m_pTarget = nullptr;
}

void UCombatComponent::e_StunStart(float fStunTime)
{
	Owner->setUnitState(EUNIT_STATE::E_UnderAttack);
	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage, 1.f); // 0.5로하면 기존 스피드 1/2배
		AnimInstance->Montage_JumpToSection(FName("Stun"), CombatMontage);
		FTimerHandle DestroyTimer{};
		GetWorld()->GetTimerManager().SetTimer(DestroyTimer, this, &UCombatComponent::e_StunEnd, fStunTime, false, (-1.0F));
	}
}

void UCombatComponent::e_StunEnd()
{
	Owner->setUnitState(EUNIT_STATE::E_Idle);
	if (AnimInstance && CombatMontage)
	{
		if (AnimInstance->Montage_GetCurrentSection(CombatMontage) == FName("Stun"))
		{
			AnimInstance->Montage_Stop(1.f, CombatMontage);
		}
	}
}

void UCombatComponent::comboActive()
{
	AnimInstance = Owner->GetMesh()->GetAnimInstance();
	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage, 1.f); // 0.5로하면 기존 스피드 1/2배
		AnimInstance->Montage_JumpToSection(m_ComboSectionName, CombatMontage);
	}
}

bool UCombatComponent::CanCombo()
{
	if (m_bIsComboOn && m_ComboSectionName != FName("None"))
		return true;
	return false;
}


void UCombatComponent::setInit(AUnitBase* pTarget, ECharType eTargetType)
{
	m_pTarget = pTarget;
	m_eTargetType = eTargetType;
}

void UCombatComponent::clearTarget()
{
	m_pTarget = nullptr;
	m_eTargetType = ECharType::E_None;
	Owner->setUnitState(EUNIT_STATE::E_Idle);
}