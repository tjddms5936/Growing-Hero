// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_SkillWindow.h"
#include "UMG_SkillSlot.h"

UUMG_SkillWindow::UUMG_SkillWindow(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	m_nMaxLength{},
	m_ppSlot{}
{

}

void UUMG_SkillWindow::NativeOnInitialized()
{
}

void UUMG_SkillWindow::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUMG_SkillWindow::init()
{
	m_nMaxLength = (int)ESkillNum::E_MAX;
	m_ppSlot.Reserve(m_nMaxLength);
	m_ppSlot.Init(nullptr, m_nMaxLength);

	m_ppSlot[(int)ESkillNum::E_Skill_1] = Slot_Skill1;
	m_ppSlot[(int)ESkillNum::E_Skill_2] = Slot_Skill2;
	m_ppSlot[(int)ESkillNum::E_Skill_3] = Slot_Skill3;
	m_ppSlot[(int)ESkillNum::E_Skill_4] = Slot_Skill4;
	m_ppSlot[(int)ESkillNum::E_Skill_5] = Slot_Skill5;
	m_ppSlot[(int)ESkillNum::E_Skill_6] = Slot_Skill6;

	for (auto SkillSlot : m_ppSlot)
	{
		SkillSlot->init();
	}

	/*m_ppSlot[(int)ESkillNum::E_Skill_1]->setMyHotKey(EKEY::E_1);
	m_ppSlot[(int)ESkillNum::E_Skill_1]->enterHotKey();*/
}

void UUMG_SkillWindow::UpdateSkillSlot(ESkillNum eSkillNum)
{
	if (m_ppSlot.IsValidIndex((int)eSkillNum))
	{
		m_ppSlot[(int)eSkillNum]->UpdateSlot();
	}
}

void UUMG_SkillWindow::RefreshWindow()
{
	for (auto mySlot : m_ppSlot)
	{
		mySlot->UpdateSlot();
	}
}

TMap<int32, int32> UUMG_SkillWindow::getEnteredHotKeyInfo()
{
	TMap<int32, int32> mapResult{};
	for (auto mySlot : m_ppSlot)
	{
		if (mySlot->m_eMyHotKey != EKEY::E_Default)
		{
			mapResult.Add((int32)mySlot->m_eSkillNum, (int32)mySlot->m_eMyHotKey);
		}
	}
	return mapResult;
}

void UUMG_SkillWindow::LoadSkillHotKey(const TMap<int32, int32>& mapData)
{
	for (auto HotKeyInfo : mapData)
	{
		if (m_ppSlot.IsValidIndex(HotKeyInfo.Key) && IsValid(m_ppSlot[HotKeyInfo.Key]))
		{
			m_ppSlot[HotKeyInfo.Key]->setMyHotKey((EKEY)HotKeyInfo.Value);
			m_ppSlot[HotKeyInfo.Key]->enterHotKey();
		}
	}
}
