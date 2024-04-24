// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_Inven_EquipmentSlot.h"
#include "GrowingHero/MyCharacter.h"
#include "GrowingHero/MyCharacterController.h"
#include "GrowingHero/InventoryComponent.h"
#include "GrowingHero/InterfaceProp_Inventory.h"
#include "Interface_Window_Slot.h"


UUMG_Inven_EquipmentSlot::UUMG_Inven_EquipmentSlot(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{

}

void UUMG_Inven_EquipmentSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UUMG_Inven_EquipmentSlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUMG_Inven_EquipmentSlot::Activate()
{
	UseItem();
}

void UUMG_Inven_EquipmentSlot::clearSlot()
{
	m_pMyController->InventorySystemComponent->ClearItem(m_eItemType, m_FEuqipItem.ItemName, m_nSlotIndex);

	m_bEmpty = true;
	m_pThumbnail = m_pDefaultThumbnail;

	ToolTipToggle();
	m_FEuqipItem.ClearStructure();
	m_FEuqipItem.Thumbnail = m_pDefaultThumbnail;
}

void UUMG_Inven_EquipmentSlot::AddItem(FName ItemRowName)
{
	if ((ItemDB->FindRow<FInventoryEquipment>(ItemRowName, ItemRowName.ToString()) == nullptr))
	{
		GEngine->AddOnScreenDebugMessage(1, 2, FColor::Black, FString::Printf(TEXT("No Item in DB")));
		return;
	}

	m_FEuqipItem = *(ItemDB->FindRow<FInventoryEquipment>(ItemRowName, ItemRowName.ToString()));

	m_bEmpty = false;
	m_pThumbnail = m_FEuqipItem.Thumbnail;
	GEngine->AddOnScreenDebugMessage(1, 2, FColor::Black, FString::Printf(TEXT("Add Item!")));
}

void UUMG_Inven_EquipmentSlot::UseItem()
{
	if (!ensure(m_FEuqipItem.EquipItemClass != nullptr))
	{
		return;
	}

	// 장비 레벨보다 캐릭터 레벨 낮으면 착용 불가
	if (m_FEuqipItem.ReqLV > m_pMyHero->getUnitStat()->Level)
	{
		GEngine->AddOnScreenDebugMessage(1, 1.5, FColor::Red, FString::Printf(TEXT("Item level higher than your level!")));
		return;
	}

	// 아이템 사용 효과 시전
	FTransform SpawnTransform{};
	SpawnTransform = m_pMyController->GetCharacter()->GetMesh()->GetSocketTransform(FName("DropItemPoint"));
	AInterfaceProp_Inventory* SpawnedItem = GetWorld()->SpawnActor<AInterfaceProp_Inventory>(m_FEuqipItem.FieldItemClass.Get(), SpawnTransform);
	if (SpawnedItem)
	{
		SpawnedItem->setInterfacedUnit(m_pMyHero);
		SpawnedItem->ActivateSystem();
		
		// 일단 필드 아이템 소환시키고 그 아이템이 장비아이템을 장착시켜줘야한다. 그래서 필드아이템은 없어져야 함
		SpawnedItem->DestroyActor();
	}
	
	m_pMyController->InventorySystemComponent->SubtractItem(m_eItemType, m_FEuqipItem.ItemName, m_nSlotIndex);
	UpdataSlot();
}

void UUMG_Inven_EquipmentSlot::DropItem()
{
	if (!ensure(m_FEuqipItem.FieldItemClass != nullptr))
		return;

	// 필드로 스폰
	FVector DropLocation = m_pMyHero->GetMesh()->GetSocketLocation(FName("DropItemPoint"));
	FTransform SpawnTransform{};
	SpawnTransform = GetWorld()->GetFirstPlayerController()->GetCharacter()->GetMesh()->GetSocketTransform(FName("DropItemPoint"));

	UWorld* world = GetWorld();
	world->SpawnActor<AInterfaceProp_Inventory>(m_FEuqipItem.FieldItemClass.Get(), SpawnTransform);

	 m_pMyController->InventorySystemComponent->SubtractItem(m_eItemType, m_FEuqipItem.ItemName, m_nSlotIndex);
	UpdataSlot();
}

void UUMG_Inven_EquipmentSlot::SwapSlot(int32 nDragSlotIndex)
{
	if (!ensure(m_WindowSlotInterface != nullptr))
		return;
	m_WindowSlotInterface->SwapSlot(m_eItemType, nDragSlotIndex, m_nSlotIndex);
}

void UUMG_Inven_EquipmentSlot::UpdataSlot()
{
	if (m_pMyController->InventorySystemComponent->m_arEquipmentItem[m_nSlotIndex].IsDefault)
	{
		clearSlot();
		return;
	}
	m_FEuqipItem.CopyStructure(m_pMyController->InventorySystemComponent->m_arEquipmentItem[m_nSlotIndex]);
	m_bEmpty = false;
}

void UUMG_Inven_EquipmentSlot::init()
{
	Super::init();
	m_eItemType = EItemType::E_EQUIPMENT;
	m_bIsCountable = false;
	m_FEuqipItem.Thumbnail = m_pDefaultThumbnail;
}

