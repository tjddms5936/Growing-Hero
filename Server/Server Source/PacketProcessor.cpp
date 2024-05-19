#include "pch.h"
#include "PacketProcessor.h"
#include "SendBuffer.h"
#include "ClientPacketHandler.h"
#include "ChildSessionMGR.h"
#include "Struct.pb.h"
#include "RoomMGR.h"
#include "UnitBase.h"
#include "DBPool.h"
#include "DB.h"

PacketProcessor GPktProcessor;

// Ŭ�� C_LOGIN ��Ŷ ������ ó���ϴ� �κ�.
void PacketProcessor::Work_C_LOGIN(int32 sessionID, Protocol::C_LOGIN& pkt)
{
	std::string utf8_string = pkt.playername();  // ���� ���, Protocol Buffer���� ������ ���ڿ�
	std::wstring utf16_string = utf8_to_wstring(pkt.playername());
	cout << "Login ID : ";
	WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), utf16_string.c_str(), utf16_string.size(), NULL, NULL);

	bool IsOkNickName = true;
	do
	{
		// DB �۾�
		// ������ �˻� (�ߺ� ID ����)
		// TODO
		DB* DBConnection{};
		DBConnection = GDB_Pool->Pop();
		DBConnection->Unbind();

		// �ѱ� ���� ���ε�
		SQLWCHAR NickName[256];
		wcsncpy_s(NickName, utf16_string.c_str(), _TRUNCATE);
		SQLLEN len = SQL_NTS;
		ASSERT_CRASH(DBConnection->BindParam(1, SQL_C_WCHAR, SQL_WVARCHAR, wcslen(NickName), NickName, &len));

		// SQL ����
		ASSERT_CRASH(DBConnection->Query(L"SELECT COUNT(*) FROM [dbo].[NickName] WHERE [NickName] = ?"));

		// ��� ó��
		SQLINTEGER count = 0;
		SQLLEN indicator = 0;
		ASSERT_CRASH(DBConnection->BindCol(1, SQL_C_SLONG, sizeof(count), &count, &indicator));

		if (DBConnection->Fetch())
		{
			if (count > 0) 
			{
				std::wcout << L"NickName already exists: " << utf16_string << std::endl;
				GDB_Pool->Push(DBConnection);

				// TODO : �ش� �г������δ� �����ߴٴ� ��Ŷ �����ؾ���
				IsOkNickName = false;
				break; // �ߺ��̹Ƿ� �� �̻� �������� ����
			}
		}
		else 
		{
			// Fetch ���� ���� ó��
			std::wcout << L"Failed to fetch data." << std::endl;
			GDB_Pool->Push(DBConnection);

			// TODO : �ߺ� �г����� �ƴѵ�.. ���� �߸���. 
			IsOkNickName = false;
			break; // �ߺ��̹Ƿ� �� �̻� �������� ����
		}
		// �ߺ��� �ƴϸ� �����͸� �߰�
		DBConnection->Unbind();

		// �ѱ� ���� ���ε�
		ASSERT_CRASH(DBConnection->BindParam(1, SQL_C_WCHAR, SQL_WVARCHAR, wcslen(NickName), NickName, &len));

		// SQL ����
		ASSERT_CRASH(DBConnection->Query(L"INSERT INTO [dbo].[NickName]([NickName]) VALUES(?)"));

		GDB_Pool->Push(DBConnection);

	} while (false);

	// Ŭ�� �α��� ��û. sessionID�� �ش� ��� sessionID�̴�. �״�� Ŭ������ ����.
	Protocol::S_LOGIN sendPkt;
	sendPkt.set_success(IsOkNickName);
	sendPkt.set_yoursessionid(sessionID);
	sendPkt.set_playername(pkt.playername());
	// ����� Ŭ������ 1:1�� ������� ��.
	shared_ptr<SendBuffer> sendBuffer = ClientPacketHandler::MakeSendBuffer(sendPkt);
	GChildSessionMGR.OneToOneSend(sendBuffer, sessionID);
}



void PacketProcessor::Work_C_ENTER_ROOM(int32 sessionID, Protocol::C_ENTER_ROOM& pkt)
{
	// ����
	Protocol::PlayerInfo* NewPlayerinfo = new Protocol::PlayerInfo;
	NewPlayerinfo->set_player_id(sessionID);
	NewPlayerinfo->set_room(pkt.info().room());
	NewPlayerinfo->set_x(GetRandomNum(0.f, 100.f));
	NewPlayerinfo->set_y(GetRandomNum(0.f, 100.f));
	NewPlayerinfo->set_z(200.f);
	

	// �ش� �濡 ���� �߰�
	shared_ptr<RoomBase> Room = GRoomMGR.GetRoom(pkt.info().room());
	Room->AddUnit(sessionID, *NewPlayerinfo);
	GChildSessionMGR.GetSession(sessionID)->SetPlayerRoomType(pkt.info().room());

	shared_ptr<UnitBase> NewPlayer = Room->GetUnit(sessionID);
	NewPlayer->SetPlayerName(pkt.playername());

	// ���ο� Ŭ������ ��ġ �������ֱ�
	{
		Protocol::S_ENTER_ROOM pkt2;

		Protocol::PlayerInfo* playerInfo = new Protocol::PlayerInfo();
		playerInfo->CopyFrom(NewPlayer->GetPlayerInfo());
		pkt2.set_allocated_info(playerInfo);
		shared_ptr<SendBuffer> sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt2);
		GChildSessionMGR.OneToOneSend(sendBuffer, sessionID);
	}
	
	// �����̸�.. ���� ���� �������� ��
	if (Room->GetRoomType() != Protocol::ROOM_TUTORIAL)
		return;

	// �ش� Room ��� Ŭ������ S_Spawn �����ֱ�.(���ο� Ŭ�� ���� �˸�)
	{
		Protocol::S_SPAWN pkt2;
		Protocol::PlayerInfo* info = pkt2.add_info();
		info->CopyFrom(NewPlayer->GetPlayerInfo());
		shared_ptr<SendBuffer> sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt2);
		GChildSessionMGR.BroadcastRoom(sendBuffer, sessionID, pkt.info().room());
	}
	
	// �ش� Room ��� Ŭ�� ������ NewŬ������ �����ֱ�
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
	// ���� �� ��� Ŭ������ ��Ŷ ������
	{
		Protocol::S_DESPAWN pkt2;
		pkt2.set_player_id(pkt.info().player_id());
		shared_ptr<SendBuffer> sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt2);
		GChildSessionMGR.BroadcastRoom(sendBuffer, pkt.info().player_id(), pkt.info().room());
	}

	// ���� �뿡�� �ش� Ŭ�� ����������
	shared_ptr<RoomBase> Room = GRoomMGR.GetRoom(pkt.info().room());
	Room->RemoveUnit(pkt.info().player_id());
}

void PacketProcessor::Work_C_MOVE(Protocol::C_MOVE& pkt)
{
	const uint64 PlayerID = pkt.info().player_id();
	shared_ptr<RoomBase> PlayerRoom = GRoomMGR.GetRoom(pkt.info().room());
	if (PlayerRoom->IsUnitInRoom(PlayerID) == false)
		return;

	// ����
	shared_ptr<UnitBase> Player = PlayerRoom->GetUnit(PlayerID);
	Player->UpdateInfo(pkt.info());

	// �̵�
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
	// �𸮾󿡼� UTF-8�� ��ȯ ��Ų������ ������ �׳� ���� �ȴ�. 
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
	// �ᱹ ���� ����� Room�� �ִ� Player ������ OK.
	// �ش� Session ������ OK
	// ���� ���� Room �ٸ� Ŭ������� Despawn �����ָ�ɵ�

	// ���� �� ��� Ŭ������ ��Ŷ ������
	{
		Protocol::S_DESPAWN pkt2;
		pkt2.set_player_id(pkt.info().player_id());
		shared_ptr<SendBuffer> sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt2);
		GChildSessionMGR.BroadcastRoom(sendBuffer, pkt.info().player_id(), pkt.info().room());
	}

}






