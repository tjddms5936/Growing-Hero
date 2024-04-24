// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface_Window_Slot.h"
#include "UMG_HotKeyWindow.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API UUMG_HotKeyWindow : public UUserWidget, public IInterface_Window_Slot
{
	GENERATED_BODY()
	
public:
	UUMG_HotKeyWindow(const FObjectInitializer& ObjectInitializer); // 생성자
	virtual void NativeOnInitialized() override; // 위젯이 생성될 때 딱 한 번 호출된다. 에디터 편집 시에도 생성될 때 호출된다
	virtual void SwapSlot(EItemType eItemType, int32 nDragSlotIndex, int32 nDropSlotIndex) override;
	virtual void UpdateSlot(int32 nidx) override;
public:
	int32 m_nMaxLength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SlotArray")
	TArray<UUMG_HotKeySlot*> m_ppHotKeySlot;

	UFUNCTION(BlueprintCallable)
	class UUMG_HotKeySlot* getHotKeySlotFromIndex(int32 nHotKeySlotIndex);

private:
	void ppHotKeySlotInit();
	class AMyCharacterController* m_pMyController;

public:
	void init();
	void LoadHotKey();
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_HotKeySlot* Slot_1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_HotKeySlot* Slot_2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_HotKeySlot* Slot_3;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_HotKeySlot* Slot_4;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_HotKeySlot* Slot_5;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_HotKeySlot* Slot_6;
};


