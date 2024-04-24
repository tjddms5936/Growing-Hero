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
	// RoomBase��(��) ���� ��ӵ�
	virtual shared_ptr<UnitBase> GetUnit(uint64 UnitSessionID) override;

private:
	// �������� ���� �ʿ伺 ����� �ؾ� �� ��. 

	//vector<shared_ptr<UnitBase>> m_UnitPool;
	//unordered_map<int32, shared_ptr<UnitBase>> m_mapUnits;
};

