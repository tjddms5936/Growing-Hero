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
	// GChildSessionMGR에 자기자신 등록
	GChildSessionMGR.AddSession(shared_from_this());
	
}

void GameSession::OnDisconnect()
{
	// 현재 플레이어 룸에서 Session이 관리하는 player 지워주기
	GRoomMGR.GetRoom(m_Room_PlayerdIn)->RemoveUnit(GetSessionID());


	// GChildSessionMGR에서 자기자신 해제
	GChildSessionMGR.ReleaseSession(shared_from_this());
}

bool GameSession::OnRecv(unsigned char* recvBuffer, int32 len)
{
	// 아래는 뭐... 받고나서 할 행동 들어가서 정의해주면 되고
	ClientPacketHandler::HandlePacket(GetSessionID(), recvBuffer, len);
	return true;
}

void GameSession::OnSend(int32 len)
{
	
}
