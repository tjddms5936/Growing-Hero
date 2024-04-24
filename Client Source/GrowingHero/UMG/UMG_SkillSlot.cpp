// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_SkillSlot.h"
#include "GrowingHero/MyCharacterController.h"
#include "GrowingHero/SkillComponent.h"

UUMG_SkillSlot::UUMG_SkillSlot(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{

}

void UUMG_SkillSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UUMG_SkillSlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUMG_SkillSlot::Activate()
{
	UseSkill();
}

UTexture2D* UUMG_SkillSlot::getThumbnail()
{
	return m_pThumbnail;
}

void UUMG_SkillSlot::init()
{
	Super::init();
	UpdateSlot();
}

void UUMG_SkillSlot::UpdateSlot()
{
	m_FSkillData.CopyStructure(m_pMyController->SkillComponent->m_arSkill[(int)m_eSkillNum]);
	m_pThumbnail = m_FSkillData.Thumbnail;
	m_nCoolTime = m_FSkillData.SKill_CoolTime;
	UpdateThumbnail();
}

bool UUMG_SkillSlot::CanLvUp()
{
	if (m_pMyController->SkillComponent->getSkillPoint() <= 0)
	{
		GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Red, FString::Printf(TEXT("스킬 포인트가 부족합니다!")));
		return false;
	}

	FName PreSkill = m_FSkillData.Pre_Skill_ID;
	if (PreSkill == FName("None"))
		return true;

	int32 index = FCString::Atof(*PreSkill.ToString());
	if (m_pMyController->SkillComponent->m_arSkill.IsValidIndex(index))
	{
		int32 PreeSkillCurLv = m_pMyController->SkillComponent->m_arSkill[index].SKill_CurLev;
		if (PreeSkillCurLv >= m_FSkillData.Pre_Skill_Lev && m_FSkillData.SKill_CurLev < m_FSkillData.Skill_MaxLev)
			return true;

		GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Red, FString::Printf(TEXT("선행 스킬을 배워야 합니다!")));
	}

	return false;
}

void UUMG_SkillSlot::LvUp()
{
	m_pMyController->SkillComponent->SKillLevelUp(m_eSkillNum);
	UpdateSlot();
}

void UUMG_SkillSlot::UseSkill()
{
	if (!m_pMyController->SkillComponent->IsSkillCanActivat(m_eSkillNum))
		return;

	m_pMyController->SkillComponent->UsingSkill(m_eSkillNum);
	if (!m_pMyController->SkillComponent->IsSkillCoolTimeOK(m_eSkillNum))
		return;

	CoolTimeStart();
	if (ED_CoolTimeStart.IsBound() == true)
	{
		ED_CoolTimeStart.Execute();
	}
}

