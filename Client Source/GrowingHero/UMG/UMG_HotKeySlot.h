// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG_HotKeySlot.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API UUMG_HotKeySlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UUMG_HotKeySlot(const FObjectInitializer& ObjectInitializer); // 생성자
	virtual void NativeOnInitialized() override; // 위젯이 생성될 때 딱 한 번 호출된다. 에디터 편집 시에도 생성될 때 호출된다
	virtual void NativeConstruct() override; // BeginPlay
public:
	UPROPERTY(meta = (BindWidget))
	class UImage* ItemImage;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HotKey Info")
	FText m_HotKeyName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HotKey Info")
	EKEY m_eKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HotKey Info")
	bool m_bEmpty;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HotKey Info")
	int32 m_nQuantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HotKey Info")
	UTexture2D* m_pThumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HotKey Info")
	UTexture2D* m_DefaultThumbnail;
	
	// 인벤토리 Slot이나 스킬 Slot이 등록될 것이다. 이중포인터는 UPROPERTY가 안된다.
	class UInterfaceWithHotKeySlot_Base** m_ppInterfaceSlot;
	
	class IInterface_Window_Slot* m_WindowSlotInterface;

private:
	class AMyCharacterController* m_pMyController;

public:
	void init();
	void setWindowSlotInterface(IInterface_Window_Slot* WindowSlot_Interface);

	UFUNCTION(BlueprintCallable)
	void Activate();
	
	UFUNCTION(BlueprintCallable)
	void clearSlot();

	UFUNCTION(BlueprintCallable)
	void SwapSlot(EKEY eDragSlotKey);
	
	UFUNCTION()
	void UpdateSlot();

	UFUNCTION(BlueprintCallable)
	void UpdateSlotOnlyForBlueprint();

	UFUNCTION(BlueprintCallable)
	void ClearAlreadyEntered(UInterfaceWithHotKeySlot_Base* pInterfaceSlot);

	UFUNCTION(BlueprintCallable)
	UInterfaceWithHotKeySlot_Base* getInterfaceSlot();

	UFUNCTION(BlueprintCallable)
	void clearBeforeSettingSlot();

	UFUNCTION(BlueprintImplementableEvent)
	void CoolTimeStart();
};
