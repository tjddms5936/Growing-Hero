#include "pch.h"
#include "UnitBase.h"
#include "RoomBase.h"


UnitBase::UnitBase()
{
	cout << "=================================" << "\n";
	cout << "UnitBase() Constructor" << "\n";
	m_UnitInfo = new Protocol::PlayerInfo();
}

UnitBase::~UnitBase()
{
	cout << "~UnitBase() Destructor" << "\n";
	delete m_UnitInfo;
}

void UnitBase::init()
{

}

void UnitBase::UpdateInfo(const Protocol::PlayerInfo& info)
{
	m_UnitInfo->CopyFrom(info);
}
