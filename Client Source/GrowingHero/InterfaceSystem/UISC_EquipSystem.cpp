// Fill out your copyright notice in the Description page of Project Settings.


#include "UISC_EquipSystem.h"
#include "GrowingHero/MyCharacter.h"
#include "GrowingHero/MyCharacterController.h"
#include "GrowingHero/EquipmentComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/DataTable.h"

UUISC_EquipSystem::UUISC_EquipSystem() :
	m_eEquipType{},
	m_FItem{},
	m_arSocketName{},
	m_nMaxLength{}
{
	m_nMaxLength = (int)EEquipType::E_MAX;
	m_arSocketName.Reserve(m_nMaxLength);
	m_arSocketName.Init(FName(), m_nMaxLength);

	m_arSocketName[(int)EEquipType::E_WEAPON] = FName("WeaponPoint");
	m_arSocketName[(int)EEquipType::E_ARMOR] = FName("ArmorPoint");
	m_arSocketName[(int)EEquipType::E_HAT] = FName("HatPoint");
	m_arSocketName[(int)EEquipType::E_SHOES] = FName("ShoesPoint");

	EquipItemDB = LoadObject<UDataTable>(nullptr, TEXT("'/Game/DataBase/InventoryItem_Equipment_DB.InventoryItem_Equipment_DB'"));
}

void UUISC_EquipSystem::ActiveSystem()
{
	AMyCharacterController* Controller = Cast<AMyCharacterController>(GetWorld()->GetFirstPlayerController());
	if (!ensure(Controller != nullptr))
		return;
	AMyCharacter* pEquipChar = Cast<AMyCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (pEquipChar == nullptr)
		return;
	// ======================================== ÀåÂø ========================================
	const FActorSpawnParameters spawnParams;
	FTransform SpawnTransform{};
	SpawnTransform = GetWorld()->GetFirstPlayerController()->GetCharacter()->GetMesh()->GetSocketTransform(FName("DropItemPoint"));

	m_pEquipItem = GetWorld()->SpawnActor<AEquipItem>(m_FItem.EquipItemClass.Get(), 
		pEquipChar->GetMesh()->GetSocketLocation(m_arSocketName[(int)m_eEquipType]),
		pEquipChar->GetMesh()->GetSocketRotation(m_arSocketName[(int)m_eEquipType]), 
		spawnParams);
	
	m_pEquipItem->AttachToComponent(pEquipChar->GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true), 
		m_arSocketName[(int)m_eEquipType]);
	// =============================== EquipmentComponent ¼¼ÆÃ ===============================
	Controller->EquipmentComponent->EnterEquipment(m_eEquipType, m_FItem, m_pEquipItem);
	Controller->EquipmentFrameRefresh();
}

void UUISC_EquipSystem::setInitSystem(EEquipType eEquipType, FName ItemRowName)
{
	m_eEquipType = eEquipType;

	if ((EquipItemDB->FindRow<FInventoryEquipment>(ItemRowName, ItemRowName.ToString()) != nullptr))
	{
		m_FItem.CopyStructure(*(EquipItemDB->FindRow<FInventoryEquipment>(ItemRowName, ItemRowName.ToString())));
	}
}

