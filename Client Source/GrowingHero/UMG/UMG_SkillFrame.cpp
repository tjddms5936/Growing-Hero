// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_SkillFrame.h"
#include "UMG_SkillWindow.h"
#include "GrowingHero/MyCharacterController.h"
#include "GrowingHero/SkillComponent.h"


UUMG_SkillFrame::UUMG_SkillFrame(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	m_nCurSkillPoint{}
{

}

void UUMG_SkillFrame::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UUMG_SkillFrame::init()
{
	Super::init();
	SkillWindow->init();
	m_nCurSkillPoint = m_pMyController->SkillComponent->getSkillPoint();
}

void UUMG_SkillFrame::Setup()
{
	Super::Setup();
}

void UUMG_SkillFrame::TearDown()
{
	Super::TearDown();
}

void UUMG_SkillFrame::UpdateSkillWindow()
{
	m_nCurSkillPoint = m_pMyController->SkillComponent->getSkillPoint();
	SkillWindow->RefreshWindow();
}

TMap<int32, int32> UUMG_SkillFrame::getEnteredHotKeyInfo()
{
	return SkillWindow->getEnteredHotKeyInfo();
}

void UUMG_SkillFrame::LoadSkillHotKey(const TMap<int32, int32>& mapData)
{
	SkillWindow->LoadSkillHotKey(mapData);
}
