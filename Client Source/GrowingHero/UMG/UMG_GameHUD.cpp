// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_GameHUD.h"
#include "UMG_InventoryFrame.h"
#include "GrowingHero/MyCharacterController.h"
#include "UMG_ChatBox.h"
#include "Components/EditableTextBox.h"

UUMG_GameHUD::UUMG_GameHUD(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer), m_bIsChatOn{}
{
	m_arGetInventoryWindow[(int)EItemType::E_CONSUMPTION] = &UUMG_GameHUD::getInventoryWindowConsumption;
	m_arGetInventoryWindow[(int)EItemType::E_EQUIPMENT] = &UUMG_GameHUD::getInventoryWindowEquipment;
	m_arGetInventoryWindow[(int)EItemType::E_ETC] = &UUMG_GameHUD::getInventoryWindowEquipment;
	
}

void UUMG_GameHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	m_pMyController = Cast<AMyCharacterController>(GetWorld()->GetFirstPlayerController());
}

UUMG_InventoryWindow* UUMG_GameHUD::getInventoryWindow(EItemType eItemType)
{
	if (!ensure(m_pMyController->InventoryFrame != nullptr))
		return nullptr;

	return (this->*m_arGetInventoryWindow[(int)eItemType])();
}

UUMG_InventoryWindow* UUMG_GameHUD::getInventoryWindowConsumption()
{
	return m_pMyController->InventoryFrame->InventoryWindowConsumption;
}

UUMG_InventoryWindow* UUMG_GameHUD::getInventoryWindowEquipment()
{
	return m_pMyController->InventoryFrame->InventoryWindowEquipment;
}

UUMG_InventoryWindow* UUMG_GameHUD::getInventoryWindowEtc()
{
	return m_pMyController->InventoryFrame->InventoryWindowEtc;
}

void UUMG_GameHUD::E_ChatOn()
{
	// 채팅박스 켜주고. 바로 입력 가능하도록 SendMsgBox에 포커스 시켜준다.
	UI_ChattingBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	UI_ChattingBox->SendMsgBox->SetFocus();
	m_bIsChatOn = true;
}

void UUMG_GameHUD::E_ChatOff()
{
	UI_ChattingBox->SetVisibility(ESlateVisibility::Hidden);
	m_bIsChatOn = false;
}

void UUMG_GameHUD::FocusChat()
{
	UI_ChattingBox->SendMsgBox->SetFocus();
}

void UUMG_GameHUD::AddChatMsg(Protocol::S_CHAT& pkt, bool IsMine)
{
	UI_ChattingBox->AddMsg(pkt, IsMine);
}



