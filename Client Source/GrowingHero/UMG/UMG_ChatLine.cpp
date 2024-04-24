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
    // FString::Printf를 사용하여 문자열 합치기
    FString CombinedMessage = FString::Printf(TEXT("%s: %s"), *SenderName.ToString(), *msg.ToString());

    // FString으로 형식화된 문자열을 FText로 변환하여 설정
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

