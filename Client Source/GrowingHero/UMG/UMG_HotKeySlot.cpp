// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_HotKeySlot.h"
#include "GrowingHero/MyCharacterController.h"
#include "GrowingHero/HotKeyComponent.h"
#include "InterfaceWithHotKeySlot_Base.h"
#include "Interface_Window_Slot.h"

UUMG_HotKeySlot::UUMG_HotKeySlot(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	m_eKey{},
	m_bEmpty{}
{

}

void UUMG_HotKeySlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UUMG_HotKeySlot::NativeConstruct()
{
	Super::NativeConstruct();
}


void UUMG_HotKeySlot::Activate()
{
	if ((*m_ppInterfaceSlot) == nullptr)
		return;
	(*m_ppInterfaceSlot)->Activate();
	UpdateSlot();
	
}

void UUMG_HotKeySlot::init()
{
	m_bEmpty = true;
	m_pMyController = Cast<AMyCharacterController>(GetWorld()->GetFirstPlayerController());
	m_ppInterfaceSlot = &(m_pMyController->HotKeyComponent->m_arHotKey[(int)m_eKey]);
}

void UUMG_HotKeySlot::setWindowSlotInterface(IInterface_Window_Slot* WindowSlot_Interface)
{
	m_WindowSlotInterface = WindowSlot_Interface;
}

void UUMG_HotKeySlot::SwapSlot(EKEY eDragSlotKey)
{
	if (!ensure(m_WindowSlotInterface != nullptr) && m_bEmpty == true)
		return;

	m_WindowSlotInterface->SwapSlot(EItemType::E_NONE, (int)eDragSlotKey, (int)m_eKey);
}

void UUMG_HotKeySlot::clearSlot()
{
	(*m_ppInterfaceSlot) = nullptr;
	m_bEmpty = true;
	m_nQuantity = 0;
	m_pThumbnail = m_DefaultThumbnail;
}

void UUMG_HotKeySlot::UpdateSlot()
{
	if ((*m_ppInterfaceSlot) == nullptr)
	{
		clearSlot();
		return;
	}
	m_bEmpty = false;
	m_pThumbnail = (*m_ppInterfaceSlot)->getThumbnail();

	(*m_ppInterfaceSlot)->ED_CoolTimeStart.BindUFunction(this, FName("CoolTimeStart"));
	if ((*m_ppInterfaceSlot)->m_bIsCountable)
	{
		m_nQuantity = (*m_ppInterfaceSlot)->m_nItemCount;
	}

}

void UUMG_HotKeySlot::UpdateSlotOnlyForBlueprint()
{
	UpdateSlot();
}

void UUMG_HotKeySlot::ClearAlreadyEntered(UInterfaceWithHotKeySlot_Base* pInterfaceSlot)
{
	if (pInterfaceSlot->m_eMyHotKey == EKEY::E_Default || pInterfaceSlot->m_eMyHotKey == EKEY::E_MAX)
	{
		UE_LOG(LogTemp, Warning, TEXT("ClearAlreadyEntered No"));
		return;
	}
	if ((int32)pInterfaceSlot->m_eMyHotKey < 0 || (int32)pInterfaceSlot->m_eMyHotKey >= (int32)EKEY::E_MAX)
	{
		UE_LOG(LogTemp, Warning, TEXT("ClearAlreadyEntered  but out of ranged"));
		return;
	}
	int32 clearIdx = (int32)pInterfaceSlot->m_eMyHotKey;
	UE_LOG(LogTemp, Warning, TEXT("Before Entered %d"), clearIdx);
	// 똑같은 아이템이 단축키에 세팅되어 있는 경우 정리해주는 함수
	m_pMyController->HotKeyComponent->m_arHotKey[clearIdx]->clearHotKey();
	m_WindowSlotInterface->UpdateSlot(clearIdx);
	/*for (auto v : m_pMyController->HotKeyComponent->m_arHotKey)
	{
		if (v == pInterfaceSlot)
		{
			v->clearHotKey();
			return;
		}
	}*/
}

UInterfaceWithHotKeySlot_Base* UUMG_HotKeySlot::getInterfaceSlot()
{
	return *m_ppInterfaceSlot;
}

void UUMG_HotKeySlot::clearBeforeSettingSlot()
{
	// 해당 Slot에 이미 등록된 단축키가 있다면 정리해주기
	if (getInterfaceSlot() == nullptr)
		return;

	getInterfaceSlot()->clearHotKey();
}




