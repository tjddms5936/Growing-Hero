#include "pch.h"
#include "RoomBase.h"
#include "UnitBase.h"

void RoomBase::init()
{
	m_PlayersCnt = 0;
}

bool RoomBase::AddUnit(int32 UnitSessionID, Protocol::PlayerInfo info)
{
	if (UnitSessionID < 0) // 인덱스 범위 확인
		return false;


	
	shared_ptr<UnitBase> unit = make_shared<UnitBase>();
	unit->init();
	unit->UpdateInfo(info);

	m_mapUnits.insert({ UnitSessionID, unit });
	m_PlayersCnt++;
	cout << "Add Unit SessionID : " << UnitSessionID << "\n";
	cout << "Players cnt : " << m_PlayersCnt << "\n";
	cout << "Cur Room Type : " << m_RoomType << "\n";
	return true;
}

void RoomBase::RemoveUnit(int32 UnitSessionID)
{
	// 접근 가능 map에선 지워주고
	m_mapUnits.erase(UnitSessionID);
	m_PlayersCnt--;
	cout << "Cur Room Type : " << m_RoomType << "\n";
	cout << "Players cnt : " << m_PlayersCnt << "\n";
	cout << "Remove Unit SessionID : " << UnitSessionID << "\n";
	cout << "=================================" << "\n";
}

shared_ptr<UnitBase> RoomBase::GetUnit(uint64 UnitSessionID)
{
	if (m_mapUnits.find(UnitSessionID) == m_mapUnits.end())
		return nullptr;
	return m_mapUnits[UnitSessionID];
}
