// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GrowingHero/EnumCollection.h"
#include "GrowingHero/StructureCollection.h"
#include "UMG_EquipmentSlot.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API UUMG_EquipmentSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UUMG_EquipmentSlot(const FObjectInitializer& ObjectInitializer); // 생성자
	virtual void NativeOnInitialized() override; // 위젯이 생성될 때 딱 한 번 호출된다. 에디터 편집 시에도 생성될 때 호출된다
	virtual void NativeConstruct() override; // BeginPlay

public:
	UPROPERTY(meta = (BindWidget))
	class UImage* ItemImage;

private:
	class AMyCharacterController* m_pMyController;
	class AMyCharacter* m_pMyHero;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipmentSlot|Info")
	EEquipType m_eEquipType;
	 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EquipmentSlot|Info")
	bool m_bEmpty;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EquipmentSlot|Info")
	FInventoryEquipment m_FEquipment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipmentSlot|Info")
	UTexture2D* m_pThumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipmentSlot|Info")
	UTexture2D* m_pDefaultThumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipmentSlot|Info")
	FText m_EquipName;

public:
	void init();

	UFUNCTION(BlueprintCallable)
	void UnEquipItem();

	UFUNCTION(BlueprintCallable)
	void EquipItem(class UUMG_Inven_EquipmentSlot* pDragSlot);

	UFUNCTION(BlueprintCallable)
	void DropItem();

	UFUNCTION(BlueprintCallable)
	void UpdateSlot();

	UFUNCTION(BlueprintImplementableEvent)
	void TTUpdate();
};
