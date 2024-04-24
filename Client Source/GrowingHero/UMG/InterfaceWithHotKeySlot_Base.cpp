// Fill out your copyright notice in the Description page of Project Settings.


#include "InterfaceWithHotKeySlot_Base.h"
#include "GrowingHero/MyCharacterController.h"
#include "GrowingHero/HotKeyComponent.h"

UInterfaceWithHotKeySlot_Base::UInterfaceWithHotKeySlot_Base(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	m_nItemCount{},
	m_bIsCountable{},
	m_eMyHotKey{},
	m_nCoolTime{}
{
	m_bIsCountable = true;
}

void UInterfaceWithHotKeySlot_Base::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UInterfaceWithHotKeySlot_Base::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInterfaceWithHotKeySlot_Base::Activate()
{
}

void UInterfaceWithHotKeySlot_Base::init()
{
	// 스킬의 경우 아이템 갯수 개념이 없다. boolean으로 구분하자.
	m_eMyHotKey = EKEY::E_Default;
	m_pMyController = Cast<AMyCharacterController>(GetWorld()->GetFirstPlayerController());
}

void UInterfaceWithHotKeySlot_Base::setMyHotKey(EKEY eKey)
{
	m_eMyHotKey = eKey;
}

void UInterfaceWithHotKeySlot_Base::enterHotKey()
{
	if (m_eMyHotKey != EKEY::E_Default)
	{
		m_pMyController->HotKeyComponent->setHotKey(m_eMyHotKey, this);
	}
}

UTexture2D* UInterfaceWithHotKeySlot_Base::getThumbnail()
{
	return nullptr;
}

void UInterfaceWithHotKeySlot_Base::clearHotKey()
{
	if (m_eMyHotKey != EKEY::E_Default)
	{
		m_pMyController->HotKeyComponent->setHotKey(m_eMyHotKey, nullptr);
		m_eMyHotKey = EKEY::E_Default;
		ED_CoolTimeStart.Unbind();
	}
}

