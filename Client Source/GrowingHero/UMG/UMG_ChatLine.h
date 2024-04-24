// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG_ChatLine.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API UUMG_ChatLine : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UUMG_ChatLine(const FObjectInitializer& ObjectInitializer); // ������
	virtual void NativeOnInitialized() override; // ������ ������ �� �� �� �� ȣ��ȴ�. ������ ���� �ÿ��� ������ �� ȣ��ȴ�


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextBlock;

	UFUNCTION(BlueprintCallable)
	void SetChatting(FText SenderName, FText msg, bool IsMine);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateColor m_MineColor{};
};
