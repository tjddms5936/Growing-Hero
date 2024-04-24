// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG_Frame.h"
#include "UMG_InventoryFrame.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API UUMG_InventoryFrame : public UUMG_Frame
{
	GENERATED_BODY()

public:
	UUMG_InventoryFrame(const FObjectInitializer& ObjectInitializer); // 생성자
	virtual void NativeOnInitialized() override; // 위젯이 생성될 때 딱 한 번 호출된다. 에디터 편집 시에도 생성될 때 호출된다
	virtual void NativeConstruct() override; // AddToViewport 시 호출된다. NativeOnInitialized 와 달리, Viewport 에 Add 될 때마다 불린다!

private:
	// bool m_bOpenFrame;
	TArray<class UUMG_InventoryWindow*> m_arWindow;

public:
	virtual void Setup() override;
	virtual void TearDown() override;
	virtual void init() override;

	UFUNCTION(BlueprintCallable)
	void ToggleWindow(UUMG_InventoryWindow* pOpenWindow);

	// 함수포인터 배열로 인벤토리 창 각각 분리해서 업데이트 관리
	void UpdateInventoryWindow(enum class EItemType eItemType);
	void(UUMG_InventoryFrame::* m_arUpdateWindow[(int)EItemType::E_MAX])();

	// Key : 소비아이템Slot Index정보.  Value : HotKey Index 정보. (int32)EKey 이다.
	UFUNCTION(BlueprintCallable)
	TMap<int32, int32> getEnteredHotKeyInfo();
	void LoadInventoryHotKey(const TMap<int32, int32>& mapData);

	UFUNCTION(BlueprintImplementableEvent)
	void SetupBtnColor();
private:
	void UpdateWindow_Consumption();
	void UpdateWindow_Equipment();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UUMG_InventoryWindow* InventoryWindowConsumption;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UUMG_InventoryWindow* InventoryWindowEquipment;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UUMG_InventoryWindow* InventoryWindowEtc;

};
