#pragma once
#include "RoomBase.h"
#include "Protocol.pb.h"

class RoomMGR
{
public:
public:
	RoomMGR();
	~RoomMGR();
	RoomMGR(const RoomMGR&) = delete;
	RoomMGR& operator=(const RoomMGR&) = delete;

public:
	shared_ptr<RoomBase> GetRoom(Protocol::RoomType eRoomType) { return m_vRooms[eRoomType]; }

private:
	// 모든 Room을 여기서 하드코딩으로 정의해놓는다. 
	// key:RoomNum  value:Roomptr
	unordered_map<Protocol::RoomType, shared_ptr<RoomBase>> m_vRooms;
};

extern RoomMGR GRoomMGR;