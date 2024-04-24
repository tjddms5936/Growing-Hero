#pragma once
#include "RoomBase.h"
#include "UnitBase.h"
#include "Protocol.pb.h"

class UnitBase;

class RoomBase : enable_shared_from_this<RoomBase>
{
public: 
	RoomBase(Protocol::RoomType RoomType) : m_RoomType(RoomType), m_PlayersCnt{} {}
	~RoomBase() = default;
	RoomBase(const RoomBase&) = delete;
	RoomBase& operator=(const RoomBase&) = delete;

public:
	void init();

public:
	virtual bool AddUnit(int32 UnitSessionID, Protocol::PlayerInfo info);
	virtual void RemoveUnit(int32 UnitSessionID);
	virtual shared_ptr<UnitBase> GetUnit(uint64 UnitSessionID);
	virtual bool IsUnitInRoom(uint64 UnitSessionID) { return m_mapUnits.find(UnitSessionID) != m_mapUnits.end() ? true : false; }

	Protocol::RoomType GetRoomType() { return m_RoomType; }
	uint32 GetPlayersCntInRoom() { return m_PlayersCnt; }
public:
	//	Key : UnitSessionID 평균적으로 O(1)의 시간 복잡도 보장.
	unordered_map<uint64, shared_ptr<UnitBase>> m_mapUnits;
protected:
	Protocol::RoomType m_RoomType;
	uint32 m_PlayersCnt;
};

