// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_ChatLine.h"
#include "Components/TextBlock.h"

UUMG_ChatLine::UUMG_ChatLine(const FObjectInitializer& ObjectInitializer) :
    Super(ObjectInitializer), TextBlock{}
{
	
}

void UUMG_ChatLine::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UUMG_ChatLine::SetChatting(FText SenderName, FText msg, bool IsMine)
{
    // FString::Printf�� ����Ͽ� ���ڿ� ��ġ��
    FString CombinedMessage = FString::Printf(TEXT("%s: %s"), *SenderName.ToString(), *msg.ToString());

    // FString���� ����ȭ�� ���ڿ��� FText�� ��ȯ�Ͽ� ����
    if (TextBlock)
    {
        TextBlock->SetText(FText::FromString(CombinedMessage));
        if(IsMine)
            TextBlock->SetColorAndOpacity(m_MineColor);
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("SendMsgBox Error")));
    }
}

