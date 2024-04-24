// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InterfaceWithHotKeySlot_Base.h"
#include "GrowingHero/StructureCollection.h"
#include "UMG_SkillSlot.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API UUMG_SkillSlot : public UInterfaceWithHotKeySlot_Base
{
	GENERATED_BODY()

public:
	UUMG_SkillSlot(const FObjectInitializer& ObjectInitializer); // 생성자
	virtual void NativeOnInitialized() override; // 위젯이 생성될 때 딱 한 번 호출된다. 에디터 편집 시에도 생성될 때 호출된다
	virtual void NativeConstruct() override; // AddToViewport 시 호출된다. NativeOnInitialized 와 달리, Viewport 에 Add 될 때마다 불린다!

	// ================== 단축키와 Interface 되는 함수 =====================
	virtual void Activate() override;
	virtual UTexture2D* getThumbnail() override;
	virtual void init() override;
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* LevelUpBtn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkillInfo")
	ESkillNum m_eSkillNum;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FSkillData m_FSkillData;

public:
	UFUNCTION(BlueprintCallable)
	void UpdateSlot();
	
	UFUNCTION(BlueprintCallable)
	bool CanLvUp();

	UFUNCTION(BlueprintCallable)
	void LvUp();

	UFUNCTION(BlueprintCallable)
	void UseSkill();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateThumbnail();

	UFUNCTION(BlueprintImplementableEvent)
	void CoolTimeStart();
};
