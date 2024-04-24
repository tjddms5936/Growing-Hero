#include "pch.h"
#include "RoomMGR.h"
#include "RoomBase.h"
#include "VilageRoom.h"
#include "DungeonRoom.h"

RoomMGR GRoomMGR;

RoomMGR::RoomMGR()
{
	shared_ptr<RoomBase> TutorialRoom = make_shared<RoomBase>(Protocol::RoomType::ROOM_TUTORIAL);
	TutorialRoom->init();
	shared_ptr<RoomBase> FirstDungeonRoom = make_shared<RoomBase>(Protocol::RoomType::ROOM_FIRSTDUN);
	FirstDungeonRoom->init();

	m_vRooms.insert({ TutorialRoom->GetRoomType(),TutorialRoom});
	m_vRooms.insert({ FirstDungeonRoom->GetRoomType(),FirstDungeonRoom });
}

RoomMGR::~RoomMGR()
{
	m_vRooms.clear();
}
