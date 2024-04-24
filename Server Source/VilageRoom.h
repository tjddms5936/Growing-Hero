#pragma once
#include "RoomBase.h"

class VilageRoom : public RoomBase
{
public:
	VilageRoom(Protocol::RoomType RoomType) : RoomBase(RoomType) {}
	~VilageRoom() = default;
	VilageRoom(const VilageRoom&) = delete;
	VilageRoom& operator=(const VilageRoom&) = delete;
	
public:
	// RoomBase을(를) 통해 상속됨
	virtual shared_ptr<UnitBase> GetUnit(uint64 UnitSessionID) override;

private:
	// 마을만의 관리 필요성 고민좀 해야 할 듯. 

	//vector<shared_ptr<UnitBase>> m_UnitPool;
	//unordered_map<int32, shared_ptr<UnitBase>> m_mapUnits;
};

