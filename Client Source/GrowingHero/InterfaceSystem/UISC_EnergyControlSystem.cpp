// Fill out your copyright notice in the Description page of Project Settings.


#include "UISC_EnergyControlSystem.h"
#include "GrowingHero/UnitBase.h"
#include "GrowingHero/MyCharacter.h"

void UUISC_EnergyControlSystem::AddHP()
{
	m_pInterfacedUnit->getUnitStat()->m_HP += m_fAddHP;
	if (m_pInterfacedUnit->getUnitStat()->m_HP >= m_pInterfacedUnit->getUnitStat()->m_MaxHP)
	{
		m_pInterfacedUnit->getUnitStat()->m_HP = m_pInterfacedUnit->getUnitStat()->m_MaxHP;
	}
	else if (m_pInterfacedUnit->getUnitStat()->m_HP <= 0)
	{
		m_pInterfacedUnit->getUnitStat()->m_HP = 0;
	}
	GEngine->AddOnScreenDebugMessage(1, 2, FColor::Green, FString::Printf(TEXT("Player HP : %d"), m_pInterfacedUnit->getUnitStat()->m_HP));

	AMyCharacter* MyHero = Cast<AMyCharacter>(m_pInterfacedUnit);
	if (IsValid(MyHero))
		MyHero->HPWarningOff();
}

void UUISC_EnergyControlSystem::ActiveSystem()
{
	if (!ensure(m_pInterfacedUnit != nullptr))
	{
		return;
	}

	AddHP();
}

void UUISC_EnergyControlSystem::setInitSystem(float fInitValue)
{
	setAddHP(fInitValue);
}

void UUISC_EnergyControlSystem::setAddHP(float fAddHP)
{
	m_fAddHP = fAddHP;
}
