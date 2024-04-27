#include "pch.h"
#include "ChildSessionMGR.h"
#include "RoomMGR.h"

ChildSessionMGR GChildSessionMGR;

// Ŭ�� ���ӵ� �� SessionID�� �ο��Ǵµ�.. ���ӵǾ��ٰ� �ݼ��� �� Ŭ������ '�� Session ID�� �̰ž�' ��� ������
// �� �ڷκ��� Ŭ�� ��Ŷ ������ �ڱ� Session ID�� ���� ������ �ǰڴ�. �װɷ� �Ǵ�����
// ����� �����Ǵ� SessionID�� 1���� ����ȴ�. ��, execeptID == 0�̶�� ���� �� Broadcast
void ChildSessionMGR::Broadcast(shared_ptr<SendBuffer> sendBuffer, uint32 execeptID)
{
	// ��� �����Ǵ� Session������ ��ü ����
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
	// Ư�� Room���Ը� ��ü ����
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
	// �ڱ�� ����� ������ 1:1�� �����°�
	map<int32, shared_ptr<GameSession>>::iterator iter = m_mapGameSession.find(SenderSessionID);
	if (iter != m_mapGameSession.end())
	{
		iter->second->Send(sendBuffer);
	}

}
