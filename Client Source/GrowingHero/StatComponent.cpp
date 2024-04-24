// Fill out your copyright notice in the Description page of Project Settings.


#include "StatComponent.h"
#include "Blueprint/UserWidget.h"
#include "UnitBase.h"
#include "MyCharacter.h"

// Sets default values for this component's properties
UStatComponent::UStatComponent() :
	m_mapCharStat{},
	m_nStatPoint{}
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	m_nStatPoint = 10;

	m_mapCharStat.Add(EStatName::E_Level, 0.f);
	m_mapCharStat.Add(EStatName::E_LvUpExp, 0.f);
	m_mapCharStat.Add(EStatName::E_CurExp, 0.f);
	m_mapCharStat.Add(EStatName::E_MaxHP, 0.f);
	m_mapCharStat.Add(EStatName::E_CurHP, 0.f);
	m_mapCharStat.Add(EStatName::E_MaxMP, 0.f);
	m_mapCharStat.Add(EStatName::E_CurMP, 0.f);
	m_mapCharStat.Add(EStatName::E_ATK, 0.f);
	m_mapCharStat.Add(EStatName::E_DEF, 0.f);
	m_mapCharStat.Add(EStatName::E_STR, 0.f);
	m_mapCharStat.Add(EStatName::E_DEX, 0.f);
	m_mapCharStat.Add(EStatName::E_INT, 0.f);
	m_mapCharStat.Add(EStatName::E_LUK, 0.f);
	m_mapCharStat.Add(EStatName::E_AttackRange, 0.f);
	m_mapCharStat.Add(EStatName::E_AttackSpeed, 0.f);
	m_mapCharStat.Add(EStatName::E_MAX, 0.f);
}


// Called when the game starts
void UStatComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStatComponent::init(AUnitBase* pMyChar)
{
	m_pMyHero = pMyChar;
	UpdateStat();
}

int32 UStatComponent::getStatPoint()
{
	return m_nStatPoint;
}

void UStatComponent::AddStatPoint(int32 nPoint)
{
	m_nStatPoint += nPoint;
}

void UStatComponent::StatUp(EStatName eStatName, float nStatupCount)
{
	float* pStat = m_mapCharStat.Find(eStatName);
	if (pStat != nullptr)
	{
		*pStat += nStatupCount;
	}
}

void UStatComponent::UpdateStat()
{
	m_pMyHero->getUnitStat()->Level = (int32)m_mapCharStat[EStatName::E_Level];
	m_pMyHero->getUnitStat()->m_MaxHP = (int32)m_mapCharStat[EStatName::E_MaxHP];
	m_pMyHero->getUnitStat()->m_MaxMP = (int32)m_mapCharStat[EStatName::E_MaxMP];
	m_pMyHero->getUnitStat()->ATK = (int32)m_mapCharStat[EStatName::E_ATK];
	m_pMyHero->getUnitStat()->DEF = (int32)m_mapCharStat[EStatName::E_DEF];
	m_pMyHero->getUnitStat()->STR = (int32)m_mapCharStat[EStatName::E_STR];
	m_pMyHero->getUnitStat()->DEX = (int32)m_mapCharStat[EStatName::E_DEX];
	m_pMyHero->getUnitStat()->INT = (int32)m_mapCharStat[EStatName::E_INT];
	m_pMyHero->getUnitStat()->LUK = (int32)m_mapCharStat[EStatName::E_LUK];
	m_pMyHero->getUnitStat()->AttackRange = m_mapCharStat[EStatName::E_AttackRange];
	m_pMyHero->getUnitStat()->AttackSpeed = m_mapCharStat[EStatName::E_AttackSpeed];
}

void UStatComponent::StatGameSaveSetting()
{
	m_mapCharStat[EStatName::E_LvUpExp] = (float)m_pMyHero->getUnitStat()->LvUpEXP;
	m_mapCharStat[EStatName::E_CurExp] = (float)m_pMyHero->getUnitStat()->CurEXP;
	m_mapCharStat[EStatName::E_CurHP] = (float)m_pMyHero->getUnitStat()->m_HP;
	m_mapCharStat[EStatName::E_CurMP] = (float)m_pMyHero->getUnitStat()->m_MP;
}

void UStatComponent::StatGameLoad()
{
	m_pMyHero->getUnitStat()->LvUpEXP = (int32)m_mapCharStat[EStatName::E_LvUpExp];
	m_pMyHero->getUnitStat()->CurEXP = (int32)m_mapCharStat[EStatName::E_CurExp];
	m_pMyHero->getUnitStat()->m_HP = (int32)m_mapCharStat[EStatName::E_CurHP];
	m_pMyHero->getUnitStat()->m_MP = (int32)m_mapCharStat[EStatName::E_CurMP];
	UpdateStat();
}
