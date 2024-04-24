#include "pch.h"
#include "ChildSessionMGR.h"
#include "RoomMGR.h"

ChildSessionMGR GChildSessionMGR;

// 클라가 접속될 때 SessionID가 부여되는데.. 접속되었다고 반송할 때 클라한테 '니 Session ID는 이거야' 라고 보내고
// 그 뒤로부터 클라가 패킷 보낼때 자기 Session ID도 같이 보내면 되겠다. 그걸로 판단하자
// 참고로 관리되는 SessionID는 1부터 진행된다. 즉, execeptID == 0이라면 전부 다 Broadcast
void ChildSessionMGR::Broadcast(shared_ptr<SendBuffer> sendBuffer, uint32 execeptID)
{
	// 모든 관리되는 Session들한테 전체 전송
	lock_guard<mutex> guard(m);
	for (auto iter : m_mapGameSession)
	{
		if (iter.second->GetSessionID() != execeptID)
		{
			iter.second->Send(sendBuffer);
		}
	}
}

void ChildSessionMGR::BroadcastRoom(shared_ptr<SendBuffer> sendBuffer, uint64 execeptID, Protocol::RoomType eRoomType)
{
	// 특정 Room에게만 전체 전송
	lock_guard<mutex> guard(m);
	shared_ptr<RoomBase> room = GRoomMGR.GetRoom(eRoomType);
	if (room->GetPlayersCntInRoom() <= 0)
		return;

	for (auto iter : m_mapGameSession)
	{
		if (iter.second->GetSessionID() == execeptID)
			continue;
		if (room->IsUnitInRoom(iter.second->GetSessionID()))
		{
			iter.second->Send(sendBuffer);
		}
	}
}

void ChildSessionMGR::OneToOneSend(shared_ptr<SendBuffer> sendBuffer, int32 SenderSessionID)
{
	// 자기랑 연결된 놈한테 1:1로 보내는거
	map<int32, shared_ptr<GameSession>>::iterator iter = m_mapGameSession.find(SenderSessionID);
	if (iter != m_mapGameSession.end())
	{
		iter->second->Send(sendBuffer);
	}

}
