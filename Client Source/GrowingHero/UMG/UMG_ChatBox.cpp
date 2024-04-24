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
	


	// �� �׳� �ٷ� ä��â�� �߰����ٱ�? ����....
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
	// �𸮾�:UTF16.   C++:UTF8.    
	// �𸮾󿡼� ������ UTF-8�� ��ȯ�ؼ� �����ְ�
	// �𸮾󿡼� �������� UTF-16���� ��ȯ�ؼ� �������.
	// ��, C++���������� �׳� ������ �״�� ��ȯ���� ����ϰ� ��ȯ���� �����ָ�ȴ�.
	

	UMainGameInstance* GameIns = Cast<UMainGameInstance>(GetGameInstance());
	if (GameIns)
	{
		// ��Ŷ ���� �����ش�. 
		Protocol::C_CHAT pkt{};

		// UTF-8 �������� ��ȯ
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
			// ä�� ��ġ�� ����ģ�Ÿ�.. GameOnly�� �ϴ��� GameAndUI�� �ϴ���.. ��忡 ���� �ٲ��ָ�ɵ�
		}
		break;
	}
	case ETextCommit::OnCleared:
	{
		// �޼��� �����Ŷ�� �ٽ� �Է� ������ ���·� ������ֱ�
		if (!m_bIsLastMsgEmpty)
		{
			SendMsgBox->SetFocus();
		}
		// �ƹ��͵� �Է� ���� ���¿��ٸ� �޼��� �ڽ� �����Ű��
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
