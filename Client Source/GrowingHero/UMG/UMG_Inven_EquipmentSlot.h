// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UMG_InventorySlot.h"
#include "GrowingHero/StructureCollection.h"
#include "UMG_Inven_EquipmentSlot.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API UUMG_Inven_EquipmentSlot : public UUMG_InventorySlot
{
	GENERATED_BODY()
	
public:
	UUMG_Inven_EquipmentSlot(const FObjectInitializer& ObjectInitializer); // 생성자
	virtual void NativeOnInitialized() override; // 위젯이 생성될 때 딱 한 번 호출된다. 에디터 편집 시에도 생성될 때 호출된다
	virtual void NativeConstruct() override; // AddToViewport 시 호출된다. NativeOnInitialized 와 달리, Viewport 에 Add 될 때마다 불린다!

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SlotInfo")
	FInventoryEquipment m_FEuqipItem;

public:
	// ================ 단축키와 Interface 될 함수들 ======================
	virtual void Activate() override;

public:
	virtual void clearSlot() override;

	virtual void AddItem(FName ItemRowName) override;

	virtual void UseItem() override;

	virtual void DropItem() override;

	virtual void SwapSlot(int32 nDragSlotIndex) override;

	// =================================== new =============================
	virtual void UpdataSlot() override;

	virtual void init() override;
};
