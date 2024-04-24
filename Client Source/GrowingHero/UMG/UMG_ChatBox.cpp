// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_ChatBox.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Kismet/GameplayStatics.h"

#include "MainGameInstance.h"
#include "ServerPacketHandler.h"
#include "MyCharacterController.h"
#include "UMG_ChatLine.h"
#include "ServerSession.h"
#include "MyCharacter.h"

UUMG_ChatBox::UUMG_ChatBox(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	
}

void UUMG_ChatBox::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	SendMsgBox->OnTextCommitted.AddDynamic(this, &UUMG_ChatBox::OnChatTextCommitted);
}

void UUMG_ChatBox::AddMsg(Protocol::S_CHAT& pkt, bool IsMine)
{
	Protocol::ChatSenderInfo Sender = pkt.senderinfo();
	FString playerName = UTF8_TO_TCHAR(Sender.sendername().c_str());
	FText TextPlayerName = FText::FromString(playerName);

	FString Message = UTF8_TO_TCHAR(pkt.msg().c_str());
	FText TextMessage = FText::FromString(Message);
	


	// 아 그냥 바로 채팅창에 추가해줄까? ㅇㅇ....
	UUMG_ChatLine* NewWidget = CreateWidget<UUMG_ChatLine>(GetWorld(), ChatLineClass);
	if (NewWidget)
	{
		NewWidget->SetChatting(TextPlayerName, TextMessage, IsMine);
		ScrollBox->AddChild(NewWidget);
		ScrollBox->ScrollToEnd();
	}
}

void UUMG_ChatBox::SendMsg(const FText& Text)
{
	// 언리얼:UTF16.   C++:UTF8.    
	// 언리얼에서 보낼때 UTF-8로 변환해서 보내주고
	// 언리얼에서 받을때는 UTF-16으로 변환해서 사용하자.
	// 즉, C++서버에서는 그냥 받은거 그대로 변환없이 사용하고 변환없이 보내주면된다.
	

	UMainGameInstance* GameIns = Cast<UMainGameInstance>(GetGameInstance());
	if (GameIns)
	{
		// 패킷 만들어서 보내준다. 
		Protocol::C_CHAT pkt{};

		// UTF-8 형식으로 변환
		FString TextString = Text.ToString();
		std::string UTF8String = TCHAR_TO_UTF8(*TextString);
		pkt.set_msg(UTF8String);
	
		Protocol::ChatSenderInfo* ChatInfo = new Protocol::ChatSenderInfo();
		std::string UTF8PlayerNameString = TCHAR_TO_UTF8(*GameIns->PlayerName);
		ChatInfo->set_sendername(UTF8PlayerNameString);
		ChatInfo->set_player_id(GameIns->m_ServerSession->GetSessionID());
		ChatInfo->set_room(GameIns->m_pMyHero->GetPlayerInfo().room());
		pkt.set_allocated_senderinfo(ChatInfo);

		TSharedPtr<SendBuffer> sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		GameIns->SendPacket(sendBuffer);
	}
}

void UUMG_ChatBox::OnChatTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	switch (CommitMethod)
	{
	case ETextCommit::OnEnter:
	{
		if (!Text.IsEmpty())
		{
			SendMsg(Text);
			SendMsgBox->SetText(FText());
			m_bIsLastMsgEmpty = false;
		}
		else
		{
			m_bIsLastMsgEmpty = true;
			// 채팅 안치고 엔터친거면.. GameOnly로 하던가 GameAndUI로 하던가.. 모드에 따라 바꿔주면될듯
		}
		break;
	}
	case ETextCommit::OnCleared:
	{
		// 메세지 보낸거라면 다시 입력 가능한 상태로 만들어주기
		if (!m_bIsLastMsgEmpty)
		{
			SendMsgBox->SetFocus();
		}
		// 아무것도 입력 안한 상태였다면 메세지 박스 종료시키기
		else
		{
			AMyCharacterController* Cont = Cast<AMyCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			if (Cont)
				Cont->CloseChatBox();
		}
		break;
	}
	default:
		break;
	}
}
