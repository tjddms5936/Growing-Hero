#include "pch.h"
#include "GameSession.h"
#include "ChildSessionMGR.h"
#include "ClientPacketHandler.h"
#include "Protocol.pb.h"
#include "RoomBase.h"
#include "RoomMGR.h"

GameSession::GameSession()
{
	SetSessionType(IOCP_Session::SessionType::GAME_SESSION);
}

GameSession::~GameSession()
{
	{
		Protocol::S_DESPAWN pkt2;
		pkt2.set_player_id(GetSessionID());
		shared_ptr<SendBuffer> sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt2);
		GChildSessionMGR.BroadcastRoom(sendBuffer, GetSessionID(), m_Room_PlayerdIn);
	}
	cout << "~GameSession()" << "\n";
}

void GameSession::OnConnect()
{
	// GChildSessionMGR�� �ڱ��ڽ� ���
	GChildSessionMGR.AddSession(shared_from_this());
	
}

void GameSession::OnDisconnect()
{
	// ���� �÷��̾� �뿡�� Session�� �����ϴ� player �����ֱ�
	GRoomMGR.GetRoom(m_Room_PlayerdIn)->RemoveUnit(GetSessionID());


	// GChildSessionMGR���� �ڱ��ڽ� ����
	GChildSessionMGR.ReleaseSession(shared_from_this());
}

bool GameSession::OnRecv(unsigned char* recvBuffer, int32 len)
{
	// �Ʒ��� ��... �ް��� �� �ൿ ���� �������ָ� �ǰ�
	ClientPacketHandler::HandlePacket(GetSessionID(), recvBuffer, len);
	return true;
}

void GameSession::OnSend(int32 len)
{
	
}
