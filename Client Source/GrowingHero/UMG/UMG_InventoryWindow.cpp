// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_InventoryWindow.h"
#include "GrowingHero/MyCharacterController.h"
#include "GrowingHero/InventoryComponent.h"
#include "UMG_Inven_ConsumptionSlot.h"
#include "UMG_Inven_EquipmentSlot.h"

UUMG_InventoryWindow::UUMG_InventoryWindow(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	m_nMaxLength{}
{

}

void UUMG_InventoryWindow::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UUMG_InventoryWindow::init()
{
	m_nMaxLength = 35;
	m_ppSlot.Reserve(m_nMaxLength);

	m_arSwapSlot[(int)EItemType::E_CONSUMPTION] = &UUMG_InventoryWindow::SwapSlot_Consumption;
	m_arSwapSlot[(int)EItemType::E_EQUIPMENT] = &UUMG_InventoryWindow::SwapSlot_Equipment;

	ppSlotInit(); // 모든 슬롯 배열에 저장
	for (int i = 0; i < m_nMaxLength; i++)
	{
		// Window에서는 모든 Slot의 멤버 변수인 인터페이스 포인터에 
		// 인터페이스를 상속받은 자신을 등록 시켜준다.
		m_ppSlot[i]->setWindowSlotInterface(this);
		m_ppSlot[i]->init();
	}
	m_pMyController = Cast<AMyCharacterController>(GetWorld()->GetFirstPlayerController());
}

void UUMG_InventoryWindow::UpdateWindow()
{
	for (auto slot : m_ppSlot)
	{
		slot->UpdataSlot();
	}
}

TMap<int32, int32> UUMG_InventoryWindow::getEnteredHotKeyInfo()
{
	TMap<int32, int32> mapResult{};
	for (auto mySlot : m_ppSlot)
	{
		if (mySlot->m_eMyHotKey != EKEY::E_Default)
		{
			mapResult.Add(mySlot->m_nSlotIndex, (int32)mySlot->m_eMyHotKey);
		}
	}
	return mapResult;
}

void UUMG_InventoryWindow::LoadInventoryHotKey(const TMap<int32, int32>& mapData)
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

UUMG_InventorySlot* UUMG_InventoryWindow::getEmptySlot()
{
	for (auto slot : m_ppSlot)
	{
		if (slot->m_bEmpty)
		{
			return slot;
		}
	}
	return nullptr;
}

UUMG_InventorySlot* UUMG_InventoryWindow::getSlotFromIndex(int32 nSlotIndex)
{
	if(nSlotIndex >= m_nMaxLength && nSlotIndex < 0)
		return nullptr;
	return m_ppSlot[nSlotIndex];
}


void UUMG_InventoryWindow::ppSlotInit()
{
	m_ppSlot.Init(nullptr, m_nMaxLength);
	m_ppSlot[0] = Slot_Index0;
	m_ppSlot[1] = Slot_Index1;
	m_ppSlot[2] = Slot_Index2;
	m_ppSlot[3] = Slot_Index3;
	m_ppSlot[4] = Slot_Index4;

	m_ppSlot[5] = Slot_Index5;
	m_ppSlot[6] = Slot_Index6;
	m_ppSlot[7] = Slot_Index7;
	m_ppSlot[8] = Slot_Index8;
	m_ppSlot[9] = Slot_Index9;

	m_ppSlot[10] = Slot_Index10;
	m_ppSlot[11] = Slot_Index11;
	m_ppSlot[12] = Slot_Index12;
	m_ppSlot[13] = Slot_Index13;
	m_ppSlot[14] = Slot_Index14;

	m_ppSlot[15] = Slot_Index15;
	m_ppSlot[16] = Slot_Index16;
	m_ppSlot[17] = Slot_Index17;
	m_ppSlot[18] = Slot_Index18;
	m_ppSlot[19] = Slot_Index19;

	m_ppSlot[20] = Slot_Index20;
	m_ppSlot[21] = Slot_Index21;
	m_ppSlot[22] = Slot_Index22;
	m_ppSlot[23] = Slot_Index23;
	m_ppSlot[24] = Slot_Index24;

	m_ppSlot[25] = Slot_Index25;
	m_ppSlot[26] = Slot_Index26;
	m_ppSlot[27] = Slot_Index27;
	m_ppSlot[28] = Slot_Index28;
	m_ppSlot[29] = Slot_Index29;

	m_ppSlot[30] = Slot_Index30;
	m_ppSlot[31] = Slot_Index31;
	m_ppSlot[32] = Slot_Index32;
	m_ppSlot[33] = Slot_Index33;
	m_ppSlot[34] = Slot_Index34;
}

void UUMG_InventoryWindow::SwapSlot_Consumption(int32 nDragSlotIndex, int32 nDropSlotIndex)
{
	UUMG_Inven_ConsumptionSlot* pDragSlot = Cast<UUMG_Inven_ConsumptionSlot>(m_ppSlot[nDragSlotIndex]);
	UUMG_Inven_ConsumptionSlot* pDropSlot = Cast<UUMG_Inven_ConsumptionSlot>(m_ppSlot[nDropSlotIndex]);

	// ========= 인벤창 Swap 작업 =========
	// 컨트롤러가 가지고 있는 인벤토리 컴포넌트에 접근해서 Swap 작업을 진행해준다.
	m_pMyController->InventorySystemComponent->swapSlot(m_ppSlot[nDragSlotIndex]->m_eItemType, nDragSlotIndex, nDropSlotIndex);

	pDragSlot->UpdataSlot();
	pDropSlot->UpdataSlot();

	// ========= HotKey에 알려주기 =========
	EKEY eDragSlotKey = pDragSlot->m_eMyHotKey;
	EKEY eDropSlotKey = pDropSlot->m_eMyHotKey;
	pDragSlot->setMyHotKey(eDropSlotKey);
	pDropSlot->setMyHotKey(eDragSlotKey);
	pDragSlot->enterHotKey();
	pDropSlot->enterHotKey();
}

void UUMG_InventoryWindow::SwapSlot_Equipment(int32 nDragSlotIndex, int32 nDropSlotIndex)
{
	UUMG_Inven_EquipmentSlot* pDragSlot = Cast<UUMG_Inven_EquipmentSlot>(m_ppSlot[nDragSlotIndex]);
	UUMG_Inven_EquipmentSlot* pDropSlot = Cast<UUMG_Inven_EquipmentSlot>(m_ppSlot[nDropSlotIndex]);

	// ========= 인벤창 Swap 작업 =========
	// 컨트롤러가 가지고 있는 인벤토리 컴포넌트에 접근해서 Swap 작업을 진행해준다.
	m_pMyController->InventorySystemComponent->swapSlot(m_ppSlot[nDragSlotIndex]->m_eItemType, nDragSlotIndex, nDropSlotIndex);
	pDragSlot->UpdataSlot();
	pDropSlot->UpdataSlot();
}

void UUMG_InventoryWindow::SwapSlot(EItemType eItemType, int32 nDragSlotIndex, int32 nDropSlotIndex)
{
	(this->*m_arSwapSlot[(int)eItemType])(nDragSlotIndex, nDropSlotIndex);
}

void UUMG_InventoryWindow::UpdateSlot(int32 nidx)
{
	if (nidx >= m_nMaxLength || nidx < 0)
		return;

	m_ppSlot[nidx]->UpdataSlot();
}
