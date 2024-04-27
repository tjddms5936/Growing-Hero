#include "pch.h"
#include "PacketProcessor.h"
#include "SendBuffer.h"
#include "ClientPacketHandler.h"
#include "ChildSessionMGR.h"
#include "Struct.pb.h"
#include "RoomMGR.h"
#include "UnitBase.h"

PacketProcessor GPktProcessor;

void PacketProcessor::Work_C_LOGIN(int32 sessionID, Protocol::C_LOGIN& pkt)
{
	// 클라가 로그인 요청. sessionID는 해당 담당 sessionID이다. 그대로 클라한테 전달.
	Protocol::S_LOGIN sendPkt;
	sendPkt.set_success(1);
	sendPkt.set_yoursessionid(sessionID);
	sendPkt.set_playername(pkt.playername());
	// 연결된 클라한테 1:1로 보내줘야 함.
	shared_ptr<SendBuffer> sendBuffer = ClientPacketHandler::MakeSendBuffer(sendPkt);
	GChildSessionMGR.OneToOneSend(sendBuffer, sessionID);
}



void PacketProcessor::Work_C_ENTER_ROOM(int32 sessionID, Protocol::C_ENTER_ROOM& pkt)
{
	// 적용
	Protocol::PlayerInfo* NewPlayerinfo = new Protocol::PlayerInfo;
	NewPlayerinfo->set_player_id(sessionID);
	NewPlayerinfo->set_room(pkt.info().room());
	NewPlayerinfo->set_x(GetRandomNum(0.f, 100.f));
	NewPlayerinfo->set_y(GetRandomNum(0.f, 100.f));
	NewPlayerinfo->set_z(200.f);
	

	// 해당 방에 유닛 추가
	shared_ptr<RoomBase> Room = GRoomMGR.GetRoom(pkt.info().room());
	Room->AddUnit(sessionID, *NewPlayerinfo);
	GChildSessionMGR.GetSession(sessionID)->SetPlayerRoomType(pkt.info().room());

	shared_ptr<UnitBase> NewPlayer = Room->GetUnit(sessionID);
	NewPlayer->SetPlayerName(pkt.playername());

	cout << sessionID << " Client " << "RoomType : " << pkt.info().room() << " Enter" << "\n";

	// 새로운 클라한테 위치 지정해주기
	{
		Protocol::S_ENTER_ROOM pkt2;

		Protocol::PlayerInfo* playerInfo = new Protocol::PlayerInfo();
		playerInfo->CopyFrom(NewPlayer->GetPlayerInfo());
		pkt2.set_allocated_info(playerInfo);
		shared_ptr<SendBuffer> sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt2);
		GChildSessionMGR.OneToOneSend(sendBuffer, sessionID);
	}
	
	// 던전이면.. 개인 던전 느낌으로 ㄱ
	if (Room->GetRoomType() != Protocol::ROOM_TUTORIAL)
		return;

	// 해당 Room 모든 클라한테 S_Spawn 보내주기.(새로운 클라 접속 알림)
	{
		Protocol::S_SPAWN pkt2;
		Protocol::PlayerInfo* info = pkt2.add_info();
		info->CopyFrom(NewPlayer->GetPlayerInfo());
		shared_ptr<SendBuffer> sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt2);
		GChildSessionMGR.BroadcastRoom(sendBuffer, sessionID, pkt.info().room());
	}
	
	// 해당 Room 모든 클라 정보를 New클라한테 보내주기
	{
		Protocol::S_SPAWN pkt2;
		for (std::pair<uint64, shared_ptr<UnitBase>> player : Room->m_mapUnits)
		{
			if (player.second->GetPlayerInfo().player_id() == sessionID)
				continue;

			Protocol::PlayerInfo* info = pkt2.add_info();
			info->CopyFrom(player.second->GetPlayerInfo());
		}
		if (pkt2.info_size() == 0)
		{
			return;
		}
		shared_ptr<SendBuffer> sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt2);
		GChildSessionMGR.OneToOneSend(sendBuffer, sessionID);
	}

}

void PacketProcessor::Work_C_LEAVE_ROOM(int32 sessionID, Protocol::C_LEAVE_ROOM& pkt)
{
	cout << "C_LEAVE_ROOM Start" << "\n";
	cout << "LeaveRoom Player ID : " << pkt.info().player_id() << "\n";
	cout << "LeaveRoom Player Room : " << pkt.info().room() << "\n";
	// 기존 룸 모든 클라한테 패킷 보내기
	{
		Protocol::S_DESPAWN pkt2;
		pkt2.set_player_id(pkt.info().player_id());
		shared_ptr<SendBuffer> sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt2);
		GChildSessionMGR.BroadcastRoom(sendBuffer, pkt.info().player_id(), pkt.info().room());
		cout << "Work_C_LEAVE_ROOM Send Complete" << "\n";
	}

	// 기존 룸에서 해당 클라 지워버리기
	shared_ptr<RoomBase> Room = GRoomMGR.GetRoom(pkt.info().room());
	Room->RemoveUnit(pkt.info().player_id());
}

void PacketProcessor::Work_C_MOVE(Protocol::C_MOVE& pkt)
{
	const uint64 PlayerID = pkt.info().player_id();
	shared_ptr<RoomBase> PlayerRoom = GRoomMGR.GetRoom(pkt.info().room());
	if (PlayerRoom->IsUnitInRoom(PlayerID) == false)
		return;

	// 적용
	shared_ptr<UnitBase> Player = PlayerRoom->GetUnit(PlayerID);
	Player->UpdateInfo(pkt.info());

	// 이동
	{
		Protocol::S_MOVE movePkt;
		{
			Protocol::PlayerInfo* Info = movePkt.mutable_info();
			Info->CopyFrom(pkt.info());
			movePkt.set_dashinfo(pkt.dashinfo());
		}
		shared_ptr<SendBuffer> sendBuffer = ClientPacketHandler::MakeSendBuffer(movePkt);
		GChildSessionMGR.BroadcastRoom(sendBuffer, Player->GetPlayerInfo().player_id(), Player->GetPlayerInfo().room());
	}
}

void PacketProcessor::Work_C_CHAT(Protocol::C_CHAT& pkt)
{
	// 언리얼에서 UTF-8로 변환 시킨다음에 보내서 그냥 쓰면 된다. 
	string msg = pkt.msg();
	// Protocol::ChatSenderInfo SenderInfo = pkt.senderinfo();
	Protocol::ChatSenderInfo* SenderInfo = new Protocol::ChatSenderInfo;
	SenderInfo->CopyFrom(pkt.senderinfo());

	Protocol::S_CHAT pkt2{};
	pkt2.set_msg(msg);
	pkt2.set_allocated_senderinfo(SenderInfo);
	shared_ptr<SendBuffer> sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt2);

	GChildSessionMGR.BroadcastRoom(sendBuffer, 0, SenderInfo->room());
}

void PacketProcessor::Work_C_DISCONNECT(int32 sessionID, Protocol::C_LEAVE_ROOM& pkt)
{
	// 결국 연결 끊기면 Room에 있는 Player 정리는 OK.
	// 해당 Session 정리도 OK
	// 그저 같은 Room 다른 클라들한테 Despawn 보내주면될듯

	// 기존 룸 모든 클라한테 패킷 보내기
	{
		Protocol::S_DESPAWN pkt2;
		pkt2.set_player_id(pkt.info().player_id());
		shared_ptr<SendBuffer> sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt2);
		GChildSessionMGR.BroadcastRoom(sendBuffer, pkt.info().player_id(), pkt.info().room());
	}

}
