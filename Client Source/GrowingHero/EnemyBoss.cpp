// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBoss.h"
#include "AI/BossAIController.h"
#include "UnitBase.h"
#include "CombatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/WidgetComponent.h"

AEnemyBoss::AEnemyBoss() :
	m_bDmgOn{},
	m_BossStamina{},
	m_BossMaxStamina{}
{
	AIControllerClass = ABossAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	m_DamageRange = CreateDefaultSubobject<UCapsuleComponent>(TEXT("DamageRange"));
	m_DamageRange->SetupAttachment(GetMesh(), TEXT("Weapon"));
	m_eCharType = ECharType::E_Enemy;

	Trail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail"));
	Trail->SetupAttachment(GetMesh());

	BossStatWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("BossStatWidget"));
	BossStatWidget->SetupAttachment(RootComponent);

	m_BossMaxStamina = 100.f;
	m_BossStamina = 100.f;
}

void AEnemyBoss::BeginPlay()
{
	Super::BeginPlay();

	m_DamageRange->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBoss::OnDmgOverlapBegin);
	m_DamageRange->OnComponentEndOverlap.AddDynamic(this, &AEnemyBoss::OnDmgOverlapEnd);
}

void AEnemyBoss::DieEvent()
{
	Super::DieEvent();
}


void AEnemyBoss::DestroyChar()
{
	Super::DestroyChar();
}

void AEnemyBoss::OnDmgOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!m_bDmgOn)
		return;
	if (!OtherActor->ActorHasTag("Hero"))
		return;

	AUnitBase* Target = Cast<AUnitBase>(OtherActor);
	if (!IsValid(Target))
		return;

	if (Target->getUnitState() == EUNIT_STATE::E_Dead)
		return;

	if (CombatComponent->getTarget() == Target)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack OK"));
		Target->CombatComponent->m_bHitBackOn = true;
		CombatComponent->ApplyDamage();
	}

}

void AEnemyBoss::OnDmgOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!m_bDmgOn)
		return;
	if (!OtherActor->ActorHasTag("Hero"))
		return;

	AUnitBase* Target = Cast<AUnitBase>(OtherActor);
	if (!IsValid(Target))
		return;

	if (Target->getUnitState() == EUNIT_STATE::E_Dead)
		return;

	if (CombatComponent->getTarget() == Target)
	{
		CombatComponent->m_bHitBackOn = false;
	}
}

void AEnemyBoss::NotifyActorOnClicked(FKey PressedButton)
{
	Super::NotifyActorOnClicked(PressedButton);
}

void AEnemyBoss::m_BeginTrail()
{
	Trail->BeginTrails("TB", "TE", ETrailWidthMode::ETrailWidthMode_FromFirst, 2.0f);
}

void AEnemyBoss::m_EndTrail()
{
	Trail->EndTrails();
}

