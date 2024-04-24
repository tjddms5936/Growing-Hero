// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EnumCollection.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EUNIT_STATE : uint8
{
	E_Idle UMETA(DisplayName = "Idle"),
	E_MovingToActor UMETA(DisplayName = "MovingToActor"),
	E_MovingToEnemy UMETA(DisplayName = "MovingToEnemy"),
	E_Attack UMETA(DisplayName = "Attack"),
	E_UnderAttack UMETA(DisplayName = "UnderAttack"),
	E_Dead UMETA(DisplayName = "Dead"),
	E_MAX
};

UENUM(BlueprintType)
enum class EAI_STATE : uint8
{
	E_PEACE			UMETA(DisplayName = "PEACE"), // 몬스터가 가만히 있거나, 정찰 지점으로 이동
	E_CHASE			UMETA(DisplayName = "CHASE"), // 플레이어를 감지하고 플레이어를 쫓아가기
	E_ATTACK		UMETA(DisplayName = "ATTACK"), // 공격 모션이 끝날 때까지 가만히 있기
	E_DEAD			UMETA(DisplayName = "DEAD"), // 공격 모션이 끝날 때까지 가만히 있기
	E_MAX
};

UENUM(BlueprintType)
enum class EUISC_NAME2 : uint8
{
	E_EnergyControl UMETA(DisplayName = "EnergyControl"),
	E_Equip UMETA(DisplayName = "Equip"),
	E_StatUp UMETA(DisplayName = "StatUp"),
	E_MAX
};

UENUM(BlueprintType)
enum class EItemState : uint8
{
	E_Field		UMETA(DisplayName = "Field"),
	E_Pickup	UMETA(DisplayName = "Pickup"),
	E_Equipped	UMETA(DisplayName = "Equipped"),

	E_MAX			 UMETA(DisplayName = "DefaultMax")
};

UENUM(BlueprintType)
enum class EKEY : uint8
{
	E_1 =  0	UMETA(DisplayName = "1"),
	E_2			UMETA(DisplayName = "2"),
	E_3			UMETA(DisplayName = "3"),
	E_4			UMETA(DisplayName = "4"),
	E_5			UMETA(DisplayName = "5"),
	E_6			UMETA(DisplayName = "6"),
	E_MAX		UMETA(DisplayName = "E_MAX"),
	E_Default	UMETA(DisplayName = "Default"),
	/*E_Del = 0 UMETA(DisplayName = "Delete"),
	E_Ins UMETA(DisplayName = "Insert"),
	E_End UMETA(DisplayName = "End"),
	E_Home UMETA(DisplayName = "Home"),
	E_PgUp UMETA(DisplayName = "Page Up"),
	E_PgDown UMETA(DisplayName = "Page Down"),
	E_Default UMETA(DisplayName = "Default"),*/
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	E_CONSUMPTION		UMETA(DisplayName = "Consumption"),
	E_EQUIPMENT			UMETA(DisplayName = "Equipment"),
	E_ETC				UMETA(DisplayName = "Etc"),
	E_NONE				UMETA(DisplayName = "NONE"),

	E_MAX				UMETA(DisplayName = "DefaultMax")
};

UENUM(BlueprintType)
enum class EEquipType : uint8
{
	E_HAT		UMETA(DisplayName = "Hat"),
	E_ARMOR		UMETA(DisplayName = "Armor"),
	E_WEAPON	UMETA(DisplayName = "Weapon"),
	E_SHOES		UMETA(DisplayName = "Shoes"),

	E_MAX		UMETA(DisplayName = "Max")
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	E_NoWeapon = 0		UMETA(DisplayName = "NoWeapon"),
	E_2Hand		UMETA(DisplayName = "2Hand"),

	E_MAX		UMETA(DisplayName = "Max")
};

UENUM(BlueprintType)
enum class ECharType : uint8
{
	E_Hero		UMETA(DisplayName = "Hero"),
	E_Enemy		UMETA(DisplayName = "Enemy"),
	E_None		UMETA(DisplayName = "None"),

	E_MAX			 UMETA(DisplayName = "DefaultMax")
};

UENUM(BlueprintType)
enum class ESkillNum : uint8
{
	E_Skill_1 = 0	UMETA(DisplayName = "Skill_1"),
	E_Skill_2		UMETA(DisplayName = "Skill_2"),
	E_Skill_3		UMETA(DisplayName = "Skill_3"),
	E_Skill_4		UMETA(DisplayName = "Skill_4"),
	E_Skill_5		UMETA(DisplayName = "Skill_5"),
	E_Skill_6		UMETA(DisplayName = "Skill_6"),

	E_MAX
};

UENUM(BlueprintType)
enum class ESkillType : uint8
{
	E_SummonsMovable = 0	UMETA(DisplayName = "SummonsMovable"),
	E_SummonsFix		UMETA(DisplayName = "SummonsFix"),
	E_Projectile		UMETA(DisplayName = "Projectile"),
	E_Buff		UMETA(DisplayName = "Buff"),
	E_DeBuff		UMETA(DisplayName = "DeBuff"),

	E_MAX
};

UENUM(BlueprintType)
enum class EDetectTargeType : uint8
{
	E_Owner_Specify = 0	UMETA(DisplayName = "Owner_Specify"),
	E_Owner_Range		UMETA(DisplayName = "Owner_Range"),
	E_Target_Specify		UMETA(DisplayName = "Target_Specify"),
	E_Target_Range		UMETA(DisplayName = "Target_Range"),

	E_MAX
};

UENUM(BlueprintType)
enum class EBuffNum : uint8
{
	E_Buff_1 = 0	UMETA(DisplayName = "Buff_1"),
	E_Buff_2		UMETA(DisplayName = "Buff_2"),

	E_MAX
};

UENUM(BlueprintType)
enum class EStatName : uint8
{
	E_Level = 0	UMETA(DisplayName = "Level"),
	E_LvUpExp  	UMETA(DisplayName = "LvUpExp"),
	E_CurExp  	UMETA(DisplayName = "CurExp"),
	E_MaxHP		UMETA(DisplayName = "MaxHP"),
	E_CurHP		UMETA(DisplayName = "CurHP"),
	E_MaxMP		UMETA(DisplayName = "MaxMP"),
	E_CurMP		UMETA(DisplayName = "CurMP"),
	E_ATK		UMETA(DisplayName = "ATK"),
	E_DEF		UMETA(DisplayName = "DEF"),
	E_STR		UMETA(DisplayName = "STR"),
	E_DEX		UMETA(DisplayName = "DEX"),
	E_INT		UMETA(DisplayName = "INT"),
	E_LUK		UMETA(DisplayName = "LUK"),
	E_AttackRange		UMETA(DisplayName = "AttackRange"),
	E_AttackSpeed		UMETA(DisplayName = "AttackSpeed"),

	E_MAX
};

//UENUM(BlueprintType)
//enum class EBossState : uint8
//{
//	E_Idle = 0	UMETA(DisplayName = "Idle"),
//	E_Patrol	UMETA(DisplayName = "Patrol"),
//	E_Chase		UMETA(DisplayName = "Chase"),
//	E_Attack 	UMETA(DisplayName = "Attack"),
//
//	E_MAX
//};
//
//UENUM(BlueprintType)
//enum class EBossStage : uint8
//{
//	E_Stage1 = 0	UMETA(DisplayName = "Stage1"),
//	E_Stage2		UMETA(DisplayName = "Stage2"),
//	E_Stage3 		UMETA(DisplayName = "Stage3"),
//	E_Stage4 		UMETA(DisplayName = "Stage4"),
//
//	E_MAX
//};

UCLASS()
class GROWINGHERO_API UEnumCollection : public UObject
{
	GENERATED_BODY()
	
};
