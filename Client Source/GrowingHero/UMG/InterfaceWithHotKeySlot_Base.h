// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InterfaceWithHotKeySlot_Base.generated.h"

/**
 *
 */
DECLARE_DELEGATE(FDele_Single);

UCLASS()
class GROWINGHERO_API UInterfaceWithHotKeySlot_Base : public UUserWidget
{
	GENERATED_BODY()

protected:
	class AMyCharacterController* m_pMyController;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SlotInfo")
	int32 m_nItemCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SlotInfo")
	bool m_bIsCountable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SlotInfo")
	UTexture2D* m_pThumbnail;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SlotInfo")
	EKEY m_eMyHotKey;

	FDele_Single ED_CoolTimeStart;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SlotInfo")
	int32 m_nCoolTime;
public:
	UInterfaceWithHotKeySlot_Base(const FObjectInitializer& ObjectInitializer); // 생성자
	virtual void NativeOnInitialized() override; // 위젯이 생성될 때 딱 한 번 호출된다. 에디터 편집 시에도 생성될 때 호출된다
	virtual void NativeConstruct() override; // AddToViewport 시 호출된다. NativeOnInitialized 와 달리, Viewport 에 Add 될 때마다 불린다!

	UFUNCTION(BlueprintCallable)
	virtual void Activate();
	virtual void init();

	void clearHotKey();

	UFUNCTION(BlueprintCallable)
	void setMyHotKey(EKEY eKey);

	UFUNCTION(BlueprintCallable)
	void enterHotKey();

	virtual UTexture2D* getThumbnail();
};



