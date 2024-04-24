// Fill out your copyright notice in the Description page of Project Settings.


#include "GrowingHero/UMG/UMG_InventoryFrame.h"
#include "GrowingHero/UMG/UMG_InventoryWindow.h"

UUMG_InventoryFrame::UUMG_InventoryFrame(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	m_arWindow{}
{
	
}

void UUMG_InventoryFrame::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UUMG_InventoryFrame::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUMG_InventoryFrame::Setup()
{
	Super::Setup();
	ToggleWindow(InventoryWindowConsumption);
	SetupBtnColor();
}

void UUMG_InventoryFrame::TearDown()
{
	Super::TearDown();
}

void UUMG_InventoryFrame::init()
{
	Super::init();
	m_arWindow.Reserve(3);
	m_arWindow.Init(nullptr, 3);

	InventoryWindowConsumption->init();
	InventoryWindowEquipment->init();
	InventoryWindowEtc->init();

	m_arWindow[0] = InventoryWindowConsumption;
	m_arWindow[1] = InventoryWindowEquipment;
	m_arWindow[2] = InventoryWindowEtc;

	m_arUpdateWindow[(int)EItemType::E_CONSUMPTION] = &UUMG_InventoryFrame::UpdateWindow_Consumption;
	m_arUpdateWindow[(int)EItemType::E_EQUIPMENT] = &UUMG_InventoryFrame::UpdateWindow_Equipment;
}

void UUMG_InventoryFrame::ToggleWindow(UUMG_InventoryWindow* pOpenWindow)
{
	for (auto ar : m_arWindow)
	{
		if (ar == pOpenWindow)
		{
			ar->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			ar->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UUMG_InventoryFrame::UpdateInventoryWindow(EItemType eItemType)
{
	(this->*m_arUpdateWindow[(int)eItemType])();
}

TMap<int32, int32> UUMG_InventoryFrame::getEnteredHotKeyInfo()
{
	return InventoryWindowConsumption->getEnteredHotKeyInfo();
}

void UUMG_InventoryFrame::LoadInventoryHotKey(const TMap<int32, int32>& mapData)
{
	InventoryWindowConsumption->LoadInventoryHotKey(mapData);
}

void UUMG_InventoryFrame::UpdateWindow_Consumption()
{
	InventoryWindowConsumption->UpdateWindow();
}

void UUMG_InventoryFrame::UpdateWindow_Equipment()
{
	InventoryWindowEquipment->UpdateWindow();
}
