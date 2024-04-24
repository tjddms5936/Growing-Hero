// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_EquipmentWindow.h"
#include "UMG_EquipmentSlot.h"

UUMG_EquipmentWindow::UUMG_EquipmentWindow(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	m_nMaxLength{},
	m_arEquipmentSlot{}
{

}

void UUMG_EquipmentWindow::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UUMG_EquipmentWindow::UpdateEquipmentSlot()
{
	for (auto EquipSlot : m_arEquipmentSlot)
	{
		EquipSlot->UpdateSlot();
	}
}

void UUMG_EquipmentWindow::init()
{
	m_nMaxLength = (int)EEquipType::E_MAX;
	m_arEquipmentSlot.Reserve(m_nMaxLength);
	m_arEquipmentSlot.Init(nullptr, m_nMaxLength);

	m_arEquipmentSlot[(int)EEquipType::E_HAT] = HatSlot;
	m_arEquipmentSlot[(int)EEquipType::E_ARMOR] = ArmorSlot;
	m_arEquipmentSlot[(int)EEquipType::E_WEAPON] = WeaponSlot;
	m_arEquipmentSlot[(int)EEquipType::E_SHOES] = ShoesSlot;

	for (auto EquipSlot : m_arEquipmentSlot)
	{
		EquipSlot->init();
	}
}
