// Fill out your copyright notice in the Description page of Project Settings.


#include "Summons_Fix.h"
#include "Components/SphereComponent.h"
#include "GrowingHero/EnemyCharacter.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/DamageEvents.h"


ASummons_Fix::ASummons_Fix() :
	OverlappedActors{},
	m_eUnitState{},
	m_fAttackSpeed{}
{
	m_bIsDuplicate = false;
	AttackRange = CreateDefaultSubobject<USphereComponent>(TEXT("AttackRange"));
	AttackRange->SetupAttachment(RootComponent);
	AttackRange->SetSphereRadius(500.f);
	m_fAttackSpeed = 1.5f;

	m_eUnitState = EUNIT_STATE::E_MAX;
}

void ASummons_Fix::BeginPlay()
{
	Super::BeginPlay();
	AnimInstance = SkeletalMesh->GetAnimInstance();
	if (!ensure(AnimInstance != nullptr) || !ensure(CombatMontage != nullptr))
		return;

	AnimInstance->Montage_Play(CombatMontage, 2.0f); // 2로하면 기존 스피드 2배
	AnimInstance->Montage_JumpToSection(FName("BeginSpawn"), CombatMontage);

	AttackRange->OnComponentEndOverlap.AddDynamic(this, &ASummons_Fix::AttackRangeOnOverlapEnd);

}

void ASummons_Fix::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(m_pTarget && m_pTarget->getUnitState() != EUNIT_STATE::E_Dead)
		RotateToTarget(DeltaTime);
}

void ASummons_Fix::AttackRangeOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor->ActorHasTag("Enemy"))
		return;

	m_pTarget = nullptr;
	DetectTarget();
}

void ASummons_Fix::SpawnEnd()
{
	m_eUnitState = EUNIT_STATE::E_Idle;
	ActivateSkill();
}

void ASummons_Fix::ApplyDamage()
{
	if (m_pTarget && m_pTarget->getUnitState() != EUNIT_STATE::E_Dead)
	{
		TSubclassOf<UDamageType> const ValidDamageTypeClass = DamageTypeClass ? DamageTypeClass : TSubclassOf<UDamageType>(UDamageType::StaticClass());
		FDamageEvent DamageEvent(ValidDamageTypeClass);
		m_pTarget->TakeDamage(m_nATK, DamageEvent, GetController(), this);
	}
}

void ASummons_Fix::DetectTarget()
{
	if (m_pTarget && m_pTarget->getUnitState() != EUNIT_STATE::E_Dead)
		return;

	OverlappedActors.Empty(1000);
	m_pTarget = nullptr;
	AttackRange->GetOverlappingActors(OverlappedActors, EnemyFilter);

	bool bFind{};
	for (int i = 0; i < OverlappedActors.Num(); i++)
	{
		if (!OverlappedActors[i]->ActorHasTag("Enemy"))
			continue;

		m_pTarget = Cast<AEnemyCharacter>(OverlappedActors[i]);
		if (m_pTarget->getUnitState() != EUNIT_STATE::E_Dead)
		{
			bFind = true;
			break;
		}
	}

	if (!bFind)
		m_pTarget = nullptr;
}

void ASummons_Fix::RotateToTarget(float DeltaTime)
{
	// 내가 적을 바라보는 보간 
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), m_pTarget->GetActorLocation());
	FRotator LookAtRotationYaw(0.f, LookAtRotation.Yaw, 0.f);
	FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtRotationYaw, DeltaTime, 15.f);
	
	SetActorRotation(InterpRotation);
}



void ASummons_Fix::Disappear()
{
	DestroyEvent();
	Super::Disappear();
}

void ASummons_Fix::ActivateSkill()
{
	DetectTarget();

	if (m_pTarget && m_eUnitState == EUNIT_STATE::E_Idle)
	{
		AttackTarget();
	}
	
	GetWorld()->GetTimerManager().SetTimer(DetectTimer, this, &ASummons_Fix::ActivateSkill, 1.5f, false, (-1.0F));

}

void ASummons_Fix::init(int32 nSkillLV, int32 nConsumeMP, int32 nSkillNum)
{
	Super::init(nSkillLV, nConsumeMP, nSkillNum);
	m_fHoldingTime *=  nSkillLV;
	m_nATK *= nSkillLV;
	SpanwSkillNiagara_Sound();
	m_bIsActivated = true;

	FTimerHandle DestroyTimer{};
	GetWorld()->GetTimerManager().SetTimer(DestroyTimer, this, &ASummons_Fix::CallRelease, m_fHoldingTime, false, (-1.0F));

	Super::ActivateSkill();
	ActivateSkill();
}


void ASummons_Fix::AttackTarget()
{
	if (m_pTarget && m_pTarget->getUnitState() == EUNIT_STATE::E_Dead)
		return;

	m_eUnitState = EUNIT_STATE::E_Attack;

	AnimInstance->Montage_Play(CombatMontage, 2.0f); // 0.5로하면 기존 스피드 1/2배
	AnimInstance->Montage_JumpToSection(FName("Attack"), CombatMontage);

}

void ASummons_Fix::AttackEnd()
{
	m_eUnitState = EUNIT_STATE::E_Idle;
}
