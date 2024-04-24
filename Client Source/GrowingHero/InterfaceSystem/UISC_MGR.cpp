// Fill out your copyright notice in the Description page of Project Settings.


#include "UISC_MGR.h"
#include "UISC_EnergyControlSystem.h"
#include "UISC_EquipSystem.h"
#include "UISC_StatUp.h"
#include "UserInterfaceSystemComponent.h"

// Sets default values
AUISC_MGR::AUISC_MGR()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUISC_MGR::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUISC_MGR::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUISC_MGR::createEnergyControl(UUserInterfaceSystemComponent*& pUISC)
{
	pUISC = NewObject<UUISC_EnergyControlSystem>(this);
}

void AUISC_MGR::createEquipSystem(UUserInterfaceSystemComponent*& pUISC)
{
	pUISC = NewObject<UUISC_EquipSystem>(this);
}

void AUISC_MGR::createStatUpSystem(UUserInterfaceSystemComponent*& pUISC)
{
	pUISC = NewObject<UUISC_StatUp>(this);
}


void AUISC_MGR::init()
{
	m_arFunc[(int)EUISC_NAME2::E_EnergyControl] = &AUISC_MGR::createEnergyControl;
	m_arFunc[(int)EUISC_NAME2::E_Equip] = &AUISC_MGR::createEquipSystem;
	m_arFunc[(int)EUISC_NAME2::E_StatUp] = &AUISC_MGR::createStatUpSystem;

}

UUserInterfaceSystemComponent* AUISC_MGR::createUISC(EUISC_NAME2 eUISC_Name)
{
	if (m_arFunc[(int)eUISC_Name] == nullptr)
		return nullptr;

	UUserInterfaceSystemComponent* pUISC{};
	(this->*m_arFunc[(int)eUISC_Name])(pUISC);
	return pUISC;
}

void AUISC_MGR::releaseUISC(UUserInterfaceSystemComponent*& pUISC)
{
	pUISC->ConditionalBeginDestroy();
	pUISC = nullptr;
}

