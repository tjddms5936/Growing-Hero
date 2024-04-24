// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserInterfaceSystemComponent.h"
#include "GrowingHero/EnumCollection.h"
#include "GrowingHero/StructureCollection.h"
#include "UISC_EquipSystem.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API UUISC_EquipSystem : public UUserInterfaceSystemComponent
{
	GENERATED_BODY()
public:
	UUISC_EquipSystem();
	
private:
	EEquipType m_eEquipType;
	FInventoryEquipment m_FItem;
	TArray<FName> m_arSocketName;
	int32 m_nMaxLength;
	AEquipItem* m_pEquipItem;
	const USkeletalMeshSocket* m_pAttachSocket;

	class UDataTable* EquipItemDB;
public:
	virtual void ActiveSystem() override;

	UFUNCTION(BlueprintCallable)
	void setInitSystem(EEquipType eEquipType, FName ItemRowName);
};
