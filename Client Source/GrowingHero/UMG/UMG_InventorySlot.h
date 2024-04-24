// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InterfaceWithHotKeySlot_Base.h"
//#include "GrowingHero/StructureCollection.h"
#include "GrowingHero/EnumCollection.h"
#include "UMG_InventorySlot.generated.h"

/**
 * 
 */

UCLASS()
class GROWINGHERO_API UUMG_InventorySlot : public UInterfaceWithHotKeySlot_Base
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class UImage* ItemImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* ClickBtn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SlotInfo")
	class UDataTable* ItemDB;

public:
	UUMG_InventorySlot(const FObjectInitializer& ObjectInitializer); // 생성자
	virtual void NativeOnInitialized() override; // 위젯이 생성될 때 딱 한 번 호출된다. 에디터 편집 시에도 생성될 때 호출된다
	virtual void NativeConstruct() override; // AddToViewport 시 호출된다. NativeOnInitialized 와 달리, Viewport 에 Add 될 때마다 불린다!

	// ================ 단축키와 Interface 될 함수들 ======================
	virtual void Activate() override;
	virtual void init() override;
	// ====================================================================
protected:
	class AMyCharacter* m_pMyHero;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SlotInfo")
	EItemType m_eItemType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SlotInfo")
	int32 m_nSlotIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SlotInfo")
	bool m_bEmpty;

	class IInterface_Window_Slot* m_WindowSlotInterface;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SlotInfo")
	UTexture2D* m_pDefaultThumbnail;

public:
	UFUNCTION(BlueprintCallable)
	virtual void clearSlot();

	UFUNCTION(BlueprintCallable)
	virtual void AddItem(FName ItemRowName);

	UFUNCTION(BlueprintCallable)
	virtual void UseItem();

	UFUNCTION(BlueprintCallable)
	virtual void DropItem();

	UFUNCTION(BlueprintCallable)
	virtual void SwapSlot(int32 nDragSlotIndex);

	UFUNCTION(BlueprintCallable)
	virtual	void UpdataSlot();

	

	void setWindowSlotInterface(IInterface_Window_Slot* WindowSlot_Interface);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ToolTipToggle();
};
