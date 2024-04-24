// Fill out your copyright notice in the Description page of Project Settings.


#include "UISC_StatUp.h"
#include "GrowingHero/MyCharacterController.h"
#include "GrowingHero/MyCharacter.h"
#include "GrowingHero/StatComponent.h"

UUISC_StatUp::UUISC_StatUp()
{

}

void UUISC_StatUp::ActiveSystem()
{
	AMyCharacterController* MyController = Cast<AMyCharacterController>(GetWorld()->GetFirstPlayerController());
	if (MyController == nullptr)
		return;

	AMyCharacter* MyHero = MyController->m_pMyHero;
	if (MyHero == nullptr)
		return;

	MyController->StatComponent->StatUp(EStatName::E_MaxHP, m_nMaxHP);
	MyController->StatComponent->StatUp(EStatName::E_MaxMP, m_nMaxMP);
	MyController->StatComponent->StatUp(EStatName::E_ATK, m_nATK);
	MyController->StatComponent->StatUp(EStatName::E_DEF, m_nDEF);
	MyController->StatComponent->StatUp(EStatName::E_STR, m_nSTR);
	MyController->StatComponent->StatUp(EStatName::E_DEX, m_nDEX);
	MyController->StatComponent->StatUp(EStatName::E_INT, m_nINT);
	MyController->StatComponent->StatUp(EStatName::E_LUK, m_nLUK);
	MyController->StatComponent->UpdateStat();

	MyHero->getUnitStat()->AttackRange += m_fAttackRange; // 스텟 창 보여주기용
	MyHero->getUnitStat()->AttackSpeed += m_fAttackSpeed;

	MyHero->MediateAttackRange(m_fAttackRange); // 실제 공격 범위 조정
}

void UUISC_StatUp::setInitSystem(int32 nMaxHP, int32 nMaxMP, int32 nCurEXP, int32 nATK, int32 nDEF, int32 nSTR, int32 nDEX, int32 nINT, int32 nLUK, float fAttackRange, float fAttackSpeed)
{
	m_nMaxHP = nMaxHP;
	m_nMaxMP = nMaxMP;
	m_nCurEXP = nCurEXP;
	m_nATK = nATK;
	m_nDEF = nDEF;
	m_nSTR = nSTR;
	m_nDEX = nDEX;
	m_nINT = nINT;
	m_nLUK = nLUK;
	m_fAttackRange = fAttackRange;
	m_fAttackSpeed = fAttackSpeed;
}
