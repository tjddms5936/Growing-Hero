// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG_EquipmentWindow.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API UUMG_EquipmentWindow : public UUserWidget
{
	GENERATED_BODY()

public:
	UUMG_EquipmentWindow(const FObjectInitializer& ObjectInitializer); // 생성자
	virtual void NativeOnInitialized() override; // 위젯이 생성될 때 딱 한 번 호출된다. 에디터 편집 시에도 생성될 때 호출된다
private:
	int32 m_nMaxLength;
	
public:
	TArray<class UUMG_EquipmentSlot*> m_arEquipmentSlot;

public:
	void UpdateEquipmentSlot();
	void init();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UUMG_EquipmentSlot* HatSlot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UUMG_EquipmentSlot* ArmorSlot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UUMG_EquipmentSlot* WeaponSlot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UUMG_EquipmentSlot* ShoesSlot;
};
