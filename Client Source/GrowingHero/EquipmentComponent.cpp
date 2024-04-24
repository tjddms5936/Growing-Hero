// Fill out your copyright notice in the Description page of Project Settings.

#include "EquipmentComponent.h"
#include "Blueprint/UserWidget.h"
#include "MyCharacter.h"
#include "MyCharacterController.h"
#include "InventoryComponent.h"
#include "StatComponent.h"
#include "InterfaceProp_Inventory.h"
#include "StructureCollection.h"
#include "UnitAnimInstance.h"
#include "CombatComponent.h"

// Sets default values for this component's properties
UEquipmentComponent::UEquipmentComponent() :
	m_nMaxLength{},
	m_FDefaultEquipItemInfo{},
	m_arStrEquipItem{},
	m_arEquipItem{}
{
}


// Called when the game starts
void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEquipmentComponent::init()
{
	m_FDefaultEquipItemInfo.ClearStructure();
	m_FDefaultEquipItemInfo.Thumbnail = nullptr;

	m_nMaxLength = (int)EEquipType::E_MAX;
	m_arStrEquipItem.Reserve(m_nMaxLength);
	m_arStrEquipItem.Init(m_FDefaultEquipItemInfo, m_nMaxLength);

	m_arEquipItem.Reserve(m_nMaxLength);
	m_arEquipItem.Init(nullptr, m_nMaxLength);

	m_pMyController = Cast<AMyCharacterController>(GetWorld()->GetFirstPlayerController());
	m_pMyHero = Cast<AMyCharacter>(m_pMyController->GetPawn());
	m_pMyAnimInstance = Cast<UUnitAnimInstance>(m_pMyHero->GetMesh()->GetAnimInstance());
}

void UEquipmentComponent::LoadEquipment()
{
	for (auto EquipItemStr : m_arStrEquipItem)
	{
		// 아이템 사용 효과 시전
		FTransform SpawnTransform{};
		SpawnTransform = m_pMyHero->GetMesh()->GetSocketTransform(FName("DropItemPoint"));
		AInterfaceProp_Inventory* SpawnedItem = GetWorld()->SpawnActor<AInterfaceProp_Inventory>(EquipItemStr.FieldItemClass.Get(), SpawnTransform);
		if (SpawnedItem)
		{
			SpawnedItem->setInterfacedUnit(m_pMyHero);
			SpawnedItem->ActivateSystem();

			// 일단 필드 아이템 소환시키고 그 아이템이 장비아이템을 장착시켜줘야한다. 그래서 필드아이템은 없어져야 함
			SpawnedItem->DestroyActor();
		}
	}
}



void UEquipmentComponent::EnterEquipment(EEquipType eType, FInventoryEquipment FItem, AEquipItem* pEquipItem)
{
	// 기존 장착 아이템은 인벤토리로 회수
	if (m_arEquipItem[(int)eType] != nullptr)
	{
		ReleaseEquipment(eType, false);
	}

	m_arStrEquipItem[(int)eType].CopyStructure(FItem);
	m_arEquipItem[(int)eType] = pEquipItem;

	if (eType == EEquipType::E_WEAPON)
	{
		m_pMyAnimInstance->m_bIsEquipped = true;
		m_pMyAnimInstance->m_eEquipType = m_arStrEquipItem[(int)eType].WeaponType;
		// 일단 무기는 2Hand밖에 구현 안했음
		m_pMyHero->CombatComponent->CombatMontage = m_pMyHero->CombatComponent->m_arCombatMontage[(int)m_arStrEquipItem[(int)eType].WeaponType];
	}

	m_pMyController->StatComponent->StatUp(EStatName::E_MaxHP, (float)m_arStrEquipItem[(int)eType].HP);
	m_pMyController->StatComponent->StatUp(EStatName::E_MaxMP, (float)m_arStrEquipItem[(int)eType].MP);
	m_pMyController->StatComponent->StatUp(EStatName::E_ATK, (float)m_arStrEquipItem[(int)eType].ATK);
	m_pMyController->StatComponent->StatUp(EStatName::E_DEF, (float)m_arStrEquipItem[(int)eType].DEF);
	m_pMyController->StatComponent->StatUp(EStatName::E_STR, (float)m_arStrEquipItem[(int)eType].STR);
	m_pMyController->StatComponent->StatUp(EStatName::E_DEX, (float)m_arStrEquipItem[(int)eType].DEX);
	m_pMyController->StatComponent->StatUp(EStatName::E_INT, (float)m_arStrEquipItem[(int)eType].INT);
	m_pMyController->StatComponent->StatUp(EStatName::E_LUK, (float)m_arStrEquipItem[(int)eType].LUK);
	m_pMyController->StatComponent->StatUp(EStatName::E_AttackRange, (float)m_arStrEquipItem[(int)eType].AttackRange);
	m_pMyController->StatComponent->StatUp(EStatName::E_AttackSpeed, (float)m_arStrEquipItem[(int)eType].AttackSpeed);

	m_pMyController->StatComponent->UpdateStat();
}

void UEquipmentComponent::ReleaseEquipment(EEquipType eType, bool IsDrop)
{
	// 기존 장착 아이템은 인벤토리로 회수
	if (!IsDrop)
	{
		m_pMyController->InventorySystemComponent->AddItem(EItemType::E_EQUIPMENT, m_arStrEquipItem[(int)eType].ItemName);
		m_pMyController->InventoryFrameRefresh(EItemType::E_EQUIPMENT);
	}

	m_pMyController->StatComponent->StatUp(EStatName::E_MaxHP, (float)m_arStrEquipItem[(int)eType].HP*-1);
	m_pMyController->StatComponent->StatUp(EStatName::E_MaxMP, (float)m_arStrEquipItem[(int)eType].MP*-1);
	m_pMyController->StatComponent->StatUp(EStatName::E_ATK, (float)m_arStrEquipItem[(int)eType].ATK*-1);
	m_pMyController->StatComponent->StatUp(EStatName::E_DEF, (float)m_arStrEquipItem[(int)eType].DEF*-1);
	m_pMyController->StatComponent->StatUp(EStatName::E_STR, (float)m_arStrEquipItem[(int)eType].STR*-1);
	m_pMyController->StatComponent->StatUp(EStatName::E_DEX, (float)m_arStrEquipItem[(int)eType].DEX*-1);
	m_pMyController->StatComponent->StatUp(EStatName::E_INT, (float)m_arStrEquipItem[(int)eType].INT*-1);
	m_pMyController->StatComponent->StatUp(EStatName::E_LUK, (float)m_arStrEquipItem[(int)eType].LUK*-1);
	m_pMyController->StatComponent->StatUp(EStatName::E_AttackRange, (float)m_arStrEquipItem[(int)eType].AttackRange*-1);
	m_pMyController->StatComponent->StatUp(EStatName::E_AttackSpeed, (float)m_arStrEquipItem[(int)eType].AttackSpeed * -1);
	m_pMyController->StatComponent->UpdateStat();

	// m_arStrEquipItem[(int)eType].ClearStructure();
	m_arStrEquipItem[(int)eType] = m_FDefaultEquipItemInfo;
	if (m_arEquipItem[(int)eType] != nullptr)
	{
		m_arEquipItem[(int)eType]->Destroy();
		m_arEquipItem[(int)eType] = nullptr;
	}

	if (eType == EEquipType::E_WEAPON)
	{
		m_pMyAnimInstance->m_bIsEquipped = false;
		m_pMyAnimInstance->m_eEquipType = EWeaponType::E_NoWeapon;
		m_pMyHero->CombatComponent->CombatMontage = m_pMyHero->CombatComponent->m_arCombatMontage[(int)EWeaponType::E_NoWeapon];
	}
}
