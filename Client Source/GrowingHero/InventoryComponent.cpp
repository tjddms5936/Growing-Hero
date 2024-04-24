// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Blueprint/UserWidget.h"
// #include "UMG//UMG_InventoryFrame.h"
#include "StructureCollection.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent() :
	m_nMaxInventoryLength{},
	m_arConsumptionItem{},
	m_arEquipmentItem{}
{
	PrimaryComponentTick.bCanEverTick = true;

	// UMG_InventoryWindow.cpp 에서 지정한 Slot 개수와 일치 시키기
	m_nMaxInventoryLength = 35;

}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::initInventory()
{
	for (int32 i = 0; i < m_nMaxInventoryLength; i++)
	{
		FInventoryItem FNewConsumption{};
		FInventoryEquipment FNewEquipment{};
		m_arConsumptionItem.Add(FNewConsumption);
		m_arEquipmentItem.Add(FNewEquipment);
	}
}

int32 UInventoryComponent::getMaxInventoryLength()
{
	return m_nMaxInventoryLength;
}

bool UInventoryComponent::AddItem(EItemType eItemType, FName ItemRowName)
{
	return (this->*m_arAddItem[(int)eItemType])(ItemRowName);
}

void UInventoryComponent::ClearItem(EItemType eItemType, FName ItemName, int32 SlotIndex)
{
	(this->*m_arClearItem[(int)eItemType])(ItemName, SlotIndex);
}

void UInventoryComponent::SubtractItem(EItemType eItemType, FName ItemName, int32 SlotIndex)
{
	(this->*m_arSubtractItem[(int)eItemType])(ItemName, SlotIndex);
}

void UInventoryComponent::swapSlot(EItemType eItemType, int32 nDragSlotIndex, int32 nDropSlotIndex)
{
	(this->*m_arSwapSlot[(int)eItemType])(nDragSlotIndex, nDropSlotIndex);
}

void UInventoryComponent::init()
{
	m_arAddItem[(int)EItemType::E_CONSUMPTION] = &UInventoryComponent::AddItem_Consumption;
	m_arAddItem[(int)EItemType::E_EQUIPMENT] = &UInventoryComponent::AddItem_Equipment;

	m_arSwapSlot[(int)EItemType::E_CONSUMPTION] = &UInventoryComponent::SwapSlot_Consumption;
	m_arSwapSlot[(int)EItemType::E_EQUIPMENT] = &UInventoryComponent::SwapSlot_Equipment;

	m_arClearItem[(int)EItemType::E_CONSUMPTION] = &UInventoryComponent::ClearItem_Consumption;
	m_arClearItem[(int)EItemType::E_EQUIPMENT] = &UInventoryComponent::ClearItem_Equipment;

	m_arSubtractItem[(int)EItemType::E_CONSUMPTION] = &UInventoryComponent::SubtractItem_Consumption;
	m_arSubtractItem[(int)EItemType::E_EQUIPMENT] = &UInventoryComponent::SubtractItem_Equipment;

	// ======================================== new 관리 ================================================
	m_arConsumptionItem.Reserve(m_nMaxInventoryLength);
	m_arEquipmentItem.Reserve(m_nMaxInventoryLength);

	initInventory();
}

bool UInventoryComponent::AddItem_Consumption(FName ItemRowName)
{
	// 이미 넣어본 아이템이면 여유갯수 비교해서 ++ 해주던지 새롭게 빈곳찾아 넣어주던지 해야한다. 

	// 마지막으로 저장된 동일한 아이템의 인덱스 
	int32* LSSII = m_mapConsumption_SameItem.Find(ItemRowName);

	// 이미 넣어본 아이템이고, 여유 개수 공간 있음
	if (LSSII != nullptr &&  m_arConsumptionItem[*LSSII].nCurCount < m_arConsumptionItem[*LSSII].nMaxCount)
	{
		m_arConsumptionItem[*LSSII].nCurCount++;
		return true;
	}
	
	// 처음 넣거나, 이미 넣었지만 여유 개수가 없는 경우
	// 새로운 빈 곳을 찾아서 넣어주고, 해당 아이템 map역시 이걸로 업데이트 해준다.
	for (int32 index = 0; index < m_nMaxInventoryLength; index++)
	{
		if (m_arConsumptionItem[index].IsDefault) // 빈 곳이라는 의미
		{
			FInventoryItem* FTmpItem = ConsumptionItemDB->FindRow<FInventoryItem>(ItemRowName, ItemRowName.ToString());
			if (FTmpItem == nullptr)
			{
				GEngine->AddOnScreenDebugMessage(1, 2, FColor::Black, FString::Printf(TEXT("No Item in DB")));
				return  false;
			}
			m_arConsumptionItem[index].CopyStructure(*FTmpItem);
			m_arConsumptionItem[index].IsDefault = false;
			m_arConsumptionItem[index].nCurCount++;

			// 새로 넣은 이 아이템을 map에 저장. 나중에 동일 아이템 넣을 땐 이걸 가지고 찾을거다.
			if (LSSII != nullptr)
			{
				// 이미 넣었지만 용량 초과때문. 마지막 저장 인덱스만 업데이트
				*LSSII = index;
				return true;
			}
			// 처음 넣음
			m_mapConsumption_SameItem.Add(ItemRowName, index);
			return true;

		}
	}

	return false; // 빈 곳이 없음
}

bool UInventoryComponent::AddItem_Equipment(FName ItemRowName)
{
	// 장비는 그냥 빈곳 찾아서 계속 추가해주면 된다. 
	for (int32 index = 0; index < m_nMaxInventoryLength; index++)
	{
		if (m_arEquipmentItem[index].IsDefault) // 빈 곳이라는 의미
		{
			FInventoryEquipment* FTmpItem = EquipmentItemDB->FindRow<FInventoryEquipment>(ItemRowName, ItemRowName.ToString());
			if (FTmpItem == nullptr)
			{
				GEngine->AddOnScreenDebugMessage(1, 2, FColor::Black, FString::Printf(TEXT("No Item in DB")));
				return false;
			}
			m_arEquipmentItem[index].CopyStructure(*FTmpItem);
			m_arEquipmentItem[index].IsDefault = false;

			// UI 업데이트
			// InventoryFrame->UpdateUI(EItemType::E_EQUIPMENT);

			return true;
		}
	}

	return false; // 빈 곳이 없음
}

void UInventoryComponent::SwapSlot_Consumption(int32 nDragSlotIndex, int32 nDropSlotIndex)
{
	if (!m_arConsumptionItem.IsValidIndex(nDragSlotIndex) || !m_arConsumptionItem.IsValidIndex(nDropSlotIndex))
		return;

	// Drag 아이템 백업
	FInventoryItem FTmpDragItem{};
	FTmpDragItem.CopyStructure(m_arConsumptionItem[nDragSlotIndex]);

	// Drag 아이템 Drop 아이템으로 바꾸기
	m_arConsumptionItem[nDragSlotIndex].CopyStructure(m_arConsumptionItem[nDropSlotIndex]);

	// Drop 아이템 백업해둔 Drag 아이템으로 바꾸기
	m_arConsumptionItem[nDropSlotIndex].CopyStructure(FTmpDragItem);

	// 같은 아이템 관련해서 제일 마지막으로 저장된 인덱스 관리
	int32* pIndexBackup_SameDragItem{};
	int32* pIndexBackup_SameDropItem{};

	pIndexBackup_SameDragItem = m_mapConsumption_SameItem.Find(m_arConsumptionItem[nDropSlotIndex].ItemName);
	pIndexBackup_SameDropItem = m_mapConsumption_SameItem.Find(m_arConsumptionItem[nDragSlotIndex].ItemName);
	// Drag 했던 인덱스가 스왑전 해당 아이템이 제일 마지막으로 저장되었던 인덱스였다면 업데이트 
	if (pIndexBackup_SameDragItem != nullptr && *pIndexBackup_SameDragItem == nDragSlotIndex)
		*pIndexBackup_SameDragItem = nDropSlotIndex;

	// Drop했던 인덱스가 스왑전 해당 아이템이 제일 마지막으로 저장되었던 인덱스였다면 업데이트 
	if (pIndexBackup_SameDropItem != nullptr && *pIndexBackup_SameDropItem == nDropSlotIndex)
		*pIndexBackup_SameDropItem = nDragSlotIndex;
}

void UInventoryComponent::SwapSlot_Equipment(int32 nDragSlotIndex, int32 nDropSlotIndex)
{
	if (!m_arEquipmentItem.IsValidIndex(nDragSlotIndex) || !m_arEquipmentItem.IsValidIndex(nDropSlotIndex))
		return;

	// Drag 아이템 백업
	FInventoryEquipment FTmpDragItem{};
	FTmpDragItem.CopyStructure(m_arEquipmentItem[nDragSlotIndex]);

	// Drag 아이템 Drop 아이템으로 바꾸기
	m_arEquipmentItem[nDragSlotIndex].CopyStructure(m_arEquipmentItem[nDropSlotIndex]);

	// Drop 아이템 백업해둔 Drag 아이템으로 바꾸기
	m_arEquipmentItem[nDropSlotIndex].CopyStructure(FTmpDragItem);

}

void UInventoryComponent::ClearItem_Consumption(FName ItemName, int32 SlotIndex)
{
	// 빈 곳으로 만드는것임. 
	m_arConsumptionItem[SlotIndex].ClearStructure();

	int32* LSSII = m_mapConsumption_SameItem.Find(ItemName);
	if (LSSII == nullptr || *LSSII != SlotIndex)
		return;

	m_mapConsumption_SameItem.Remove(ItemName);
}

void UInventoryComponent::ClearItem_Equipment(FName ItemName, int32 SlotIndex)
{
	// 빈 곳으로 만드는것임. 
	m_arEquipmentItem[SlotIndex].ClearStructure();
}

void UInventoryComponent::SubtractItem_Consumption(FName ItemName, int32 SlotIndex)
{
	if (m_arConsumptionItem[SlotIndex].ItemName != ItemName)
		return;

	m_arConsumptionItem[SlotIndex].nCurCount--;
	if (m_arConsumptionItem[SlotIndex].nCurCount <= 0)
	{
		m_arConsumptionItem[SlotIndex].ClearStructure();

		int32* LSSII = m_mapConsumption_SameItem.Find(ItemName);
		if (LSSII == nullptr || *LSSII != SlotIndex)
			return;
		m_mapConsumption_SameItem.Remove(ItemName);
	}
}

void UInventoryComponent::SubtractItem_Equipment(FName ItemName, int32 SlotIndex)
{
	if (m_arEquipmentItem[SlotIndex].ItemName != ItemName)
		return;

	m_arEquipmentItem[SlotIndex].ClearStructure();
}

