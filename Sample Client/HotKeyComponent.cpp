// Fill out your copyright notice in the Description page of Project Settings.


#include "HotKeyComponent.h"
#include "UMG/InterfaceWithHotKeySlot_Base.h"

// Sets default values for this component's properties
UHotKeyComponent::UHotKeyComponent() :
	m_nMaxLength{},
	m_arHotKey{}
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	m_nMaxLength = (int)EKEY::E_MAX;
	m_arHotKey.Reserve(m_nMaxLength);
	m_arHotKey.Init(m_DefaultSlot, m_nMaxLength);
}


// Called when the game starts
void UHotKeyComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UHotKeyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


void UHotKeyComponent::setHotKey(EKEY eKey, UInterfaceWithHotKeySlot_Base* pInterfaceSlot)
{
	if (pInterfaceSlot == nullptr)
		pInterfaceSlot = m_DefaultSlot;
	m_arHotKey[(int)eKey] = pInterfaceSlot;
}

void UHotKeyComponent::SwapHotKey(EKEY eDragSlotKey, EKEY eDropSlotKey)
{
	// 단축키 내에서 Swap될 때만 불리움. 
	// DragSlot은 항상 존재한다. 그래야 Drag가 된다. 하지만 DropSlot은 있을수도 없을수도 있다. 

	// 어느 경우이던 DragSlot의 정보를 백업해두어야 한다. 
	UInterfaceWithHotKeySlot_Base* pBackUpDragSlot = m_arHotKey[(int)eDragSlotKey];
	UInterfaceWithHotKeySlot_Base* pBackUpDropSlot = m_arHotKey[(int)eDropSlotKey];

	// case1 : DropSlot이 비어있을 때 
	if (m_arHotKey[(int)eDropSlotKey] == nullptr)
	{
		pBackUpDragSlot->clearHotKey();
		pBackUpDragSlot->setMyHotKey(eDropSlotKey);
		pBackUpDragSlot->enterHotKey();
	}

	else
	{
		pBackUpDragSlot->clearHotKey();
		pBackUpDragSlot->setMyHotKey(eDropSlotKey);

		pBackUpDropSlot->clearHotKey();
		pBackUpDropSlot->setMyHotKey(eDragSlotKey);
		
		pBackUpDragSlot->enterHotKey();
		pBackUpDropSlot->enterHotKey();
	}
}

void UHotKeyComponent::ActivateHotKey(EKEY eActivateKey)
{
	if (m_arHotKey[(int)eActivateKey] != nullptr)
	{
		m_arHotKey[(int)eActivateKey]->Activate();
	}
}




