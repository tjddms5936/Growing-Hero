// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "InterfaceProp_Inventory.h"
#include "EquipItem.h"
#include "Skill/SkillBase.h"
#include "EnumCollection.h"
#include "StructureCollection.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FInventoryItem : public FTableRowBase // 데이터 테이블 사용하기 위해 상속받음
{
	GENERATED_USTRUCT_BODY()
public:
	FInventoryItem() {
		ItemName = FName("Enter item name");
		nMaxCount = 0;
		nCurCount = 0;
		Use = FText::FromString("Enter effect of using");
		Description = FText::FromString("Enter description for this item");
		ItemClass = nullptr;
		Thumbnail = nullptr;
		IsDefault = true;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 nMaxCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 nCurCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Use;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AInterfaceProp_Inventory> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsDefault;

	void CopyStructure(FInventoryItem& FItem)
	{
		ItemName = FItem.ItemName;
		nMaxCount = FItem.nMaxCount;
		nCurCount = FItem.nCurCount;
		Use = FItem.Use;
		Description = FItem.Description;
		ItemClass = FItem.ItemClass;
		Thumbnail = FItem.Thumbnail;
		IsDefault = FItem.IsDefault;
	}

	void ClearStructure()
	{
		ItemName = FName();
		nMaxCount = 0;
		nCurCount = 0;
		Use = FText();
		Description = FText();
		ItemClass = nullptr;
		Thumbnail = nullptr;
		IsDefault = true;
	}
};

USTRUCT(BlueprintType)
struct FInventoryEquipment : public FTableRowBase // 데이터 테이블 사용하기 위해 상속받음
{
	GENERATED_USTRUCT_BODY()
public:
	FInventoryEquipment() {
		ItemEquipType = EEquipType::E_MAX;
		WeaponType = EWeaponType::E_MAX;
		ItemName = FName("Enter item name");
		ReqLV = 0;
		HP = 0;
		MP = 0;
		ATK = 0;
		DEF = 0;
		STR = 0;
		DEX = 0;
		INT = 0;
		LUK = 0;
		AttackRange = 0.f;
		AttackSpeed = 0.f;
		Description = FText::FromString("Enter description for this item");
		FieldItemClass = nullptr;
		EquipItemClass = nullptr;
		Thumbnail = nullptr;
		IsDefault = true;
	}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEquipType ItemEquipType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ReqLV;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 HP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ATK;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DEF;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 STR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DEX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 INT;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LUK;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AInterfaceProp_Inventory> FieldItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AEquipItem> EquipItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsDefault;

	void CopyStructure(FInventoryEquipment& FItem)
	{
		ItemEquipType = FItem.ItemEquipType;
		WeaponType = FItem.WeaponType;
		ItemName = FItem.ItemName;
		ReqLV = FItem.ReqLV;
		HP = FItem.HP;
		MP = FItem.MP;
		ATK = FItem.ATK;
		DEF = FItem.DEF;
		STR = FItem.STR;
		DEX = FItem.DEX;
		INT = FItem.INT;
		LUK = FItem.LUK;
		AttackRange = FItem.AttackRange;
		AttackSpeed = FItem.AttackSpeed;
		Description = FItem.Description;
		FieldItemClass = FItem.FieldItemClass;
		EquipItemClass = FItem.EquipItemClass;
		Thumbnail = FItem.Thumbnail;
		IsDefault = FItem.IsDefault;
	}

	void ClearStructure()
	{
		ItemEquipType = EEquipType::E_MAX;
		WeaponType = EWeaponType::E_MAX;
		ItemName = FName();
		ReqLV = 0;
		HP = 0;
		MP = 0;
		ATK = 0;
		DEF = 0;
		STR = 0;
		DEX = 0;
		INT = 0;
		LUK = 0;
		AttackRange = 0.f;
		AttackSpeed = 0.f;
		Description = FText();
		FieldItemClass = nullptr;
		EquipItemClass = nullptr;
		Thumbnail = nullptr;
		IsDefault = true;
	}
};


USTRUCT(BlueprintType)
struct FUnitStat
{
	GENERATED_USTRUCT_BODY()

public:
	FUnitStat() 
	{
		m_MaxHP = 100;
		m_HP = 100;
		m_MaxMP = 100;
		m_MP = 100;
		Level = 0;
		LvUpEXP = 100;
		CurEXP = 0;
		ATK = 0;
		DEF = 0;
		STR = 0;
		DEX = 0;
		INT = 0;
		LUK = 0;
		AttackRange = 120.f;
		AttackSpeed = 1.5f;
	}
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 m_MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 m_HP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 m_MaxMP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 m_MP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 LvUpEXP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 CurEXP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 ATK;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 DEF;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 STR;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 DEX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 INT;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 LUK;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float AttackSpeed;
};

USTRUCT(BlueprintType)
struct FItemSlotArray
{
	GENERATED_USTRUCT_BODY()
public:
	TArray<class UUMG_InventorySlot*> arItemSlot;
};

USTRUCT(BlueprintType)
struct FSkillData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FSkillData()
	{
		Skill_ID = FName("Skill ID");
		Skill_Name = FName("Skill Name");
		SKill_Des = FText::FromString("Skill Description");
		Skill_Use = FText::FromString("Skill Use Effect");
		SKill_CoolTime = 0;
		Skill_MP = 0;
		Skill_MaxLev = 10;
		SKill_CurLev = 0;
		Pre_Skill_ID = FName("None");
		Pre_Skill_Lev = 0;
		CanDuplicated = true;
		Thumbnail = nullptr;
		SpawnSkill = nullptr;
		SkillType = ESkillType::E_MAX;
		LvUpChangeSkill_CoolTime = 0;
		LvUpChangeSkill_MP = 0;
	}
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Skill_ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Skill_Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SKill_Des; // 스킬 설명. 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Skill_Use; // 스킬 사용시 효과. 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SKill_CoolTime; // 한번 사용 후 해당 시간이 지나고 나서야 해당 스킬 사용이 가능합니다.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Skill_MP; // 스킬 사용 시 필요한 MP입니다.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Skill_MaxLev;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SKill_CurLev;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Pre_Skill_ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Pre_Skill_Lev;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanDuplicated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Thumbnail;

	// 추후 클래스 디테일 변경
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ASkillBase> SpawnSkill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillType SkillType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LvUpChangeSkill_CoolTime; // 스킬 레벨 업시 변화되는 쿨타임 양입니다.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LvUpChangeSkill_MP; // 스킬 레벨 업시 변화되는 소모 MP 양입니다.

	void CopyStructure(FSkillData& FSkillData)
	{
		Skill_ID = FSkillData.Skill_ID;
		Skill_Name = FSkillData.Skill_Name;
		SKill_Des = FSkillData.SKill_Des;
		Skill_Use = FSkillData.Skill_Use;
		SKill_CoolTime = FSkillData.SKill_CoolTime;
		Skill_MP = FSkillData.Skill_MP;
		Skill_MaxLev = FSkillData.Skill_MaxLev;
		SKill_CurLev = FSkillData.SKill_CurLev;
		Pre_Skill_ID = FSkillData.Pre_Skill_ID;
		Pre_Skill_Lev = FSkillData.Pre_Skill_Lev;
		CanDuplicated = FSkillData.CanDuplicated;
		Thumbnail = FSkillData.Thumbnail;
		SpawnSkill = FSkillData.SpawnSkill;
		SkillType = FSkillData.SkillType;
		LvUpChangeSkill_CoolTime = FSkillData.LvUpChangeSkill_CoolTime;
		LvUpChangeSkill_MP = FSkillData.LvUpChangeSkill_MP;
	}

	void ClearStructure()
	{
		Skill_ID = FName("Skill ID");
		Skill_Name = FName("Skill Name");
		SKill_Des = FText::FromString("Skill Description");
		Skill_Use = FText::FromString("Skill Use Effect");
		SKill_CoolTime = 0;
		Skill_MP = 0;
		Skill_MaxLev = 10;
		SKill_CurLev = 0;
		Pre_Skill_ID = FName("None");
		Pre_Skill_Lev = 0;
		CanDuplicated = true;
		Thumbnail = nullptr;
		SpawnSkill = nullptr;
		SkillType = ESkillType::E_MAX;
		LvUpChangeSkill_CoolTime = 0;
		LvUpChangeSkill_MP = 0;
	}
};

USTRUCT(BlueprintType)
struct FQuestInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FQuestInfo()
	{
		Quest_ID = FName("Quest ID");
		Quest_Name = FName("Quest ID");
		Quest_Description = FText::FromString("Quest Description");
		Cur_Progress = 0;
		Max_Progress = 0;
		Pre_QuestID = -1;
		Next_QuestID = -1;
		Compensation_Exp = 0;
	}
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Quest_ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Quest_Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Quest_Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Cur_Progress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Max_Progress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Pre_QuestID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Next_QuestID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<TSubclassOf<AInterfaceProp_Inventory>, int32> Compensation_Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Compensation_Exp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<UTexture2D*, int32> Compensation_ItemRenderInfo;
};


USTRUCT(BlueprintType)
struct FPair_INT32
{
	GENERATED_USTRUCT_BODY()
public:
	FPair_INT32()
	{
		first = -1;
		second = -1;
	}
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 first;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 second;
};

UCLASS()
class GROWINGHERO_API UStructureCollection : public UObject
{
	GENERATED_BODY()
	
};
