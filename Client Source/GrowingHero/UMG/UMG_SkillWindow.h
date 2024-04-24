// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG_SkillWindow.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API UUMG_SkillWindow : public UUserWidget
{
	GENERATED_BODY()

public:
	UUMG_SkillWindow(const FObjectInitializer& ObjectInitializer); // 생성자
	virtual void NativeOnInitialized() override; // 위젯이 생성될 때 딱 한 번 호출된다. 에디터 편집 시에도 생성될 때 호출된다
	virtual void NativeConstruct() override; // AddToViewport 시 호출된다. NativeOnInitialized 와 달리, Viewport 에 Add 될 때마다 불린다!


private:
	int32 m_nMaxLength;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SlotArray")
	TArray<class UUMG_SkillSlot*> m_ppSlot;

private:

public:
	void init();

	UFUNCTION(BlueprintCallable)
	void UpdateSkillSlot(ESkillNum eSkillNum);

	void RefreshWindow();

	TMap<int32, int32> getEnteredHotKeyInfo();
	void LoadSkillHotKey(const TMap<int32, int32>& mapData);
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UUMG_SkillSlot* Slot_Skill1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UUMG_SkillSlot* Slot_Skill2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UUMG_SkillSlot* Slot_Skill3;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UUMG_SkillSlot* Slot_Skill4;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UUMG_SkillSlot* Slot_Skill5;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UUMG_SkillSlot* Slot_Skill6;
};
