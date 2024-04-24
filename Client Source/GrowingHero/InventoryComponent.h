// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GROWINGHERO_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	int32 m_nMaxInventoryLength;

	// 이 클래스가 해야 하는 기능은 단순하다. 인벤토리 배열을 들고있고
	// 배열에 아이템을 넣는 기능
	// UI와 상호작용하는 기능
	// 배열에서 아이템을 빼는 기능 
	// 물론 배열은 FName으로 되어있는 아이템 이름 배열이다.
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void initInventory();

	// 함수 포인터에 연결될 함수
	bool AddItem_Consumption(FName ItemRowName);
	bool AddItem_Equipment(FName ItemRowName);
	void SwapSlot_Consumption(int32 nDragSlotIndex, int32 nDropSlotIndex);
	void SwapSlot_Equipment(int32 nDragSlotIndex, int32 nDropSlotIndex);
	void ClearItem_Consumption(FName ItemName, int32 SlotIndex);
	void ClearItem_Equipment(FName ItemName, int32 SlotIndex);
	void SubtractItem_Consumption(FName ItemName, int32 SlotIndex);
	void SubtractItem_Equipment(FName ItemName, int32 SlotIndex);
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FInventoryItem> m_arConsumptionItem; 

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FInventoryEquipment> m_arEquipmentItem;

	// 소비아이템의 경우... 개수별로 관리할거니까 매번 찾는건 비효율적임. 
	// 다만 지금 구조처럼 관리하면 당장 생각나는 버그는... 용량 다 찬 아이템 한개를 버리면 그 Slot은 영영 빈 슬롯이 될 때까지 추가 안됨.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
	TMap<FName, int32> m_mapConsumption_SameItem;

	// 아이템 정보 찾아서 담기 위함
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	class UDataTable* ConsumptionItemDB; 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	class UDataTable* EquipmentItemDB; 

public:
	int32 getMaxInventoryLength();
	bool AddItem(enum class EItemType eItemType, FName ItemRowName);
	void swapSlot(enum class EItemType eItemType, int32 nDragSlotIndex, int32 nDropSlotIndex);
	void ClearItem(enum class EItemType eItemType ,FName ItemName, int32 SlotIndex);
	void SubtractItem(enum class EItemType eItemType ,FName ItemName, int32 SlotIndex);

	// 함수포인터
	bool(UInventoryComponent::* m_arAddItem[(int)EItemType::E_MAX])(FName ItemRowName);
	void(UInventoryComponent::* m_arSwapSlot[(int)EItemType::E_MAX])(int32 nDragSlotIndex, int32 nDropSlotIndex);
	void(UInventoryComponent::* m_arClearItem[(int)EItemType::E_MAX])(FName ItemName, int32 SlotIndex);
	void(UInventoryComponent::* m_arSubtractItem[(int)EItemType::E_MAX])(FName ItemName, int32 SlotIndex);

public:
	// UMG
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
	TSubclassOf<class UUserWidget> InventoryFrameClass;
	class UUMG_InventoryFrame* InventoryFrame;

	void init();
};
