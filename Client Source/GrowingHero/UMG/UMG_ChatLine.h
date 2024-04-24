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
	UUMG_ChatLine(const FObjectInitializer& ObjectInitializer); // 생성자
	virtual void NativeOnInitialized() override; // 위젯이 생성될 때 딱 한 번 호출된다. 에디터 편집 시에도 생성될 때 호출된다


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextBlock;

	UFUNCTION(BlueprintCallable)
	void SetChatting(FText SenderName, FText msg, bool IsMine);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateColor m_MineColor{};
};
