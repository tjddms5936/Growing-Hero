// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG_GameHUD.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API UUMG_GameHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UUMG_GameHUD(const FObjectInitializer& ObjectInitializer); // ������
	virtual void NativeOnInitialized() override; // ������ ������ �� �� �� �� ȣ��ȴ�. ������ ���� �ÿ��� ������ �� ȣ��ȴ�

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void init();
public:
	UFUNCTION(BlueprintCallable)
	class UUMG_InventoryWindow* getInventoryWindow(EItemType eItemType);

private:
	UUMG_InventoryWindow*(UUMG_GameHUD::* m_arGetInventoryWindow[(int)EItemType::E_MAX])();
	UUMG_InventoryWindow* getInventoryWindowConsumption();
	UUMG_InventoryWindow* getInventoryWindowEquipment();
	UUMG_InventoryWindow* getInventoryWindowEtc();


private:
	class AMyCharacterController* m_pMyController;

public:
	// =============================== ä�� �ۼ��� ���� ===============================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UUMG_ChatBox* UI_ChattingBox;

	UFUNCTION(BlueprintCallable)
	void E_ChatOn();

	UFUNCTION(BlueprintCallable)
	void E_ChatOff();

	UFUNCTION(BlueprintCallable)
	void FocusChat();

	void AddChatMsg(Protocol::S_CHAT& pkt, bool IsMine);

	bool m_bIsChatOn;
};
