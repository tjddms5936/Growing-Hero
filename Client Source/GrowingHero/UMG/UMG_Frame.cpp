// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_Frame.h"
#include "GrowingHero/MyCharacterController.h"
#include "GrowingHero/MyCharacter.h"
#include "Kismet/GameplayStatics.h"

UUMG_Frame::UUMG_Frame(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	m_bOpenFrame{}
{
	m_bOpenFrame = false;
}

void UUMG_Frame::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UUMG_Frame::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUMG_Frame::Setup()
{
	
	m_bOpenFrame = true;
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	SetIsFocusable(false);
	UGameplayStatics::SpawnSound2D(GetWorld(), SetupSound);

	if (m_pMyHero->m_bIsAdventureMode && m_pMyHero->m_nOpenedUICount <= 0)
	{
		m_pMyController->GameAndUIModeSetting();
		m_pMyController->SetShowMouseCursor(true);
	}
	m_pMyHero->m_nOpenedUICount++;

}

void UUMG_Frame::TearDown()
{
	m_bOpenFrame = false;
	SetVisibility(ESlateVisibility::Hidden);
	SetIsFocusable(false);
	UGameplayStatics::SpawnSound2D(GetWorld(), TearDownSound);
	m_pMyHero->m_nOpenedUICount--;
	if (m_pMyHero->m_bIsAdventureMode && m_pMyHero->m_nOpenedUICount <= 0)
	{
		m_pMyController->GameOnlyModeSetting();
		m_pMyController->SetShowMouseCursor(false);
	}
}

void UUMG_Frame::init()
{
	m_pMyController = Cast<AMyCharacterController>(GetWorld()->GetFirstPlayerController());
	m_pMyHero = m_pMyController->m_pMyHero;
}

bool UUMG_Frame::getToggleState()
{
    return m_bOpenFrame;
}

void UUMG_Frame::ToggleFrame()
{
	if (!m_bOpenFrame)
	{
		Setup();

		return;
	}
	TearDown();
}
