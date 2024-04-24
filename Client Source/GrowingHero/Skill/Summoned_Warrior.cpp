// Fill out your copyright notice in the Description page of Project Settings.


#include "Summoned_Warrior.h"
#include "GrowingHero/AI/SummonAIController.h"
#include "Animation/AnimInstance.h"
#include "GrowingHero/EnemyCharacter.h"
#include "GrowingHero/CombatComponent.h"

ASummoned_Warrior::ASummoned_Warrior() :
	m_sTargetingMe{}
{
	AIControllerClass = ASummonAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	m_eCharType = ECharType::E_None;
}

void ASummoned_Warrior::BeginPlay()
{
	Super::BeginPlay();

	m_eUnitState = EUNIT_STATE::E_Dead;
	if (CombatComponent->AnimInstance && CombatComponent->CombatMontage)
	{
		CombatComponent->AnimInstance->Montage_Play(CombatComponent->CombatMontage, 2.5f); // 0.5로하면 기존 스피드 1/2배
		CombatComponent->AnimInstance->Montage_JumpToSection(FName("Summon"), CombatComponent->CombatMontage);
	}
}

void ASummoned_Warrior::initSummonedStat(int nSkillLv)
{
	m_FStat.ATK *= nSkillLv;
	m_FStat.m_MaxHP *= nSkillLv;
	m_FStat.m_HP = m_FStat.m_MaxHP;
}

void ASummoned_Warrior::releaseTargetingMe()
{
	for (auto TargetingMeTarget : m_sTargetingMe)
	{
		if (TargetingMeTarget->getTarget() != nullptr && TargetingMeTarget->getTarget() == this)
		{
			TargetingMeTarget->setTargetClear();
		}
	}
}

void ASummoned_Warrior::MediateAttackRange(float fAddRangeAmount)
{
	Super::MediateAttackRange(fAddRangeAmount);
}
