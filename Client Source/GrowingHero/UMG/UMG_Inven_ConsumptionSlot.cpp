// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_Inven_ConsumptionSlot.h"
#include "GrowingHero/MyCharacter.h"
#include "GrowingHero/MyCharacterController.h"
#include "GrowingHero/InventoryComponent.h"
#include "GrowingHero/InterfaceProp_Inventory.h"
#include "Interface_Window_Slot.h"

UUMG_Inven_ConsumptionSlot::UUMG_Inven_ConsumptionSlot(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{

}

void UUMG_Inven_ConsumptionSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UUMG_Inven_ConsumptionSlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUMG_Inven_ConsumptionSlot::Activate()
{
	UseItem();
}

UTexture2D* UUMG_Inven_ConsumptionSlot::getThumbnail()
{
	return m_FItem.Thumbnail;
}

void UUMG_Inven_ConsumptionSlot::clearSlot()
{
	m_bEmpty = true;
	ToolTipToggle();
	m_FItem.ClearStructure();
	m_FItem.Thumbnail = m_pDefaultThumbnail;
	m_nItemCount = 0; // HotKey 전용 아이템 개수
}


void UUMG_Inven_ConsumptionSlot::AddItem(FName ItemRowName)
{
	if ((ItemDB->FindRow<FInventoryItem>(ItemRowName, ItemRowName.ToString()) == nullptr))
	{
		GEngine->AddOnScreenDebugMessage(1, 2, FColor::Black, FString::Printf(TEXT("No Item in DB")));
		return;
	}
	m_FItem = *(ItemDB->FindRow<FInventoryItem>(ItemRowName, ItemRowName.ToString()));

	m_nMaxItemCount = m_FItem.nMaxCount;
	m_bEmpty = false;
	m_nItemCount = 1;
	m_pThumbnail = m_FItem.Thumbnail;

	GEngine->AddOnScreenDebugMessage(1, 2, FColor::Black, FString::Printf(TEXT("Add Item!")));
}

void UUMG_Inven_ConsumptionSlot::UseItem()
{
	if (!ensure(m_FItem.ItemClass != nullptr))
		return;

	// 아이템 사용 효과 시전
	UWorld* world = GetWorld();
	FActorSpawnParameters spawnParams;
	FRotator rotator;
	AInterfaceProp_Inventory* SpawnedItem = world->SpawnActor<AInterfaceProp_Inventory>(m_FItem.ItemClass.Get(), m_pMyHero->GetActorLocation(), rotator, spawnParams);
	if (SpawnedItem)
	{
		SpawnedItem->setInterfacedUnit(m_pMyHero);
		SpawnedItem->ActivateSystem();
		SpawnedItem->DestroyActor();
	}

	 m_pMyController->InventorySystemComponent->SubtractItem(m_eItemType, m_FItem.ItemName, m_nSlotIndex);
	UpdataSlot();
	if(m_bEmpty)
		clearHotKey();
}

void UUMG_Inven_ConsumptionSlot::DropItem()
{
	if (!ensure(m_FItem.ItemClass != nullptr))
		return;

	// 아이템 필드로 스폰
	FTransform SpawnTransform{};
	SpawnTransform = GetWorld()->GetFirstPlayerController()->GetCharacter()->GetMesh()->GetSocketTransform(FName("DropItemPoint"));
	UWorld* world = GetWorld();
	world->SpawnActor<AInterfaceProp_Inventory>(m_FItem.ItemClass.Get(), SpawnTransform);

	m_pMyController->InventorySystemComponent->SubtractItem(m_eItemType, m_FItem.ItemName, m_nSlotIndex);
	UpdataSlot();
	if(m_bEmpty)
		clearHotKey();
}

void UUMG_Inven_ConsumptionSlot::SwapSlot(int32 nDragSlotIndex)
{
	if (!ensure(m_WindowSlotInterface != nullptr))
		return;
	m_WindowSlotInterface->SwapSlot(m_eItemType ,nDragSlotIndex, m_nSlotIndex);
}

void UUMG_Inven_ConsumptionSlot::UpdataSlot()
{
	if (m_pMyController->InventorySystemComponent->m_arConsumptionItem[m_nSlotIndex].IsDefault)
	{
		clearSlot();
		return;
	}
	
	m_FItem.CopyStructure(m_pMyController->InventorySystemComponent->m_arConsumptionItem[m_nSlotIndex]);
	m_bEmpty = false;
	m_nItemCount = m_FItem.nCurCount; // HotKey 등록 전용
}

void UUMG_Inven_ConsumptionSlot::init()
{
	Super::init();
	m_eItemType = EItemType::E_CONSUMPTION;
	m_FItem.Thumbnail = m_pDefaultThumbnail;
}
