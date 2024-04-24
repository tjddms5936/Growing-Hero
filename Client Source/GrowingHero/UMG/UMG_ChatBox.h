// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protocol.pb.h"
#include "UMG_ChatBox.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API UUMG_ChatBox : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UUMG_ChatBox(const FObjectInitializer& ObjectInitializer); // ������
	virtual void NativeOnInitialized() override; // ������ ������ �� �� �� �� ȣ��ȴ�. ������ ���� �ÿ��� ������ �� ȣ��ȴ�

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UScrollBox* ScrollBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UEditableTextBox* SendMsgBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
	TSubclassOf<class UUMG_ChatLine> ChatLineClass;


public:
	void AddMsg(Protocol::S_CHAT& pkt, bool IsMine);

	void SendMsg(const FText& Text);

	UFUNCTION()
	void OnChatTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	
	bool m_bIsLastMsgEmpty;
};
