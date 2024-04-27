#pragma once
#include "Protocol.pb.h"
#if UE_BUILD_DEBUG + UE_BUILD_DEVELOPMENT + UE_BUILD_TEST + UE_BUILD_SHIPPING >= 1
#include "NetworkThread.h"
#include "Buffer.h"
#define PacketHeader  FPacketHeader
#define shared_ptr  TSharedPtr
#define make_shared MakeShared
#else
#include "IOCP_Object.h"
#include "SendBuffer.h"
#endif



using PacketHandlerFunc = std::function<bool(int32, unsigned char*, int32)>;
extern PacketHandlerFunc GPacketHandlerFunc[UINT16_MAX];
#if UE_BUILD_DEBUG + UE_BUILD_DEVELOPMENT + UE_BUILD_TEST + UE_BUILD_SHIPPING >= 1
extern class UMainGameInstance* GGameInstance;
#endif

enum : uint16
{
	PKT_C_LOGIN = 1000,
	PKT_S_LOGIN = 1001,
	PKT_C_ENTER_ROOM = 1002,
	PKT_S_ENTER_ROOM = 1003,
	PKT_C_LEAVE_ROOM = 1004,
	PKT_S_SPAWN = 1005,
	PKT_S_DESPAWN = 1006,
	PKT_C_MOVE = 1007,
	PKT_S_MOVE = 1008,
	PKT_C_CHAT = 1009,
	PKT_S_CHAT = 1010,
	PKT_C_DISCONNECT = 1011,

    /*아래 부분 자동화
	PKT_S_TEST = 1,
	PKT_C_TEST = 2*/
};

bool Handle_INVALID(int32 sessionID, unsigned char* buffer, int32 len);
bool Handle_C_LOGIN(int32 sessionID, Protocol::C_LOGIN&pkt);
bool Handle_C_ENTER_ROOM(int32 sessionID, Protocol::C_ENTER_ROOM&pkt);
bool Handle_C_LEAVE_ROOM(int32 sessionID, Protocol::C_LEAVE_ROOM&pkt);
bool Handle_C_MOVE(int32 sessionID, Protocol::C_MOVE&pkt);
bool Handle_C_CHAT(int32 sessionID, Protocol::C_CHAT&pkt);
bool Handle_C_DISCONNECT(int32 sessionID, Protocol::C_DISCONNECT&pkt);

class ClientPacketHandler
{
public:
	static void init()
	{
		for (int32 i = 0; i < UINT16_MAX; i++)
			GPacketHandlerFunc[i] = Handle_INVALID;
		GPacketHandlerFunc[PKT_C_LOGIN] = [](int32 sessionID, unsigned char* buffer, int32 len)
			{
				return HandlePacket <Protocol::C_LOGIN>(Handle_C_LOGIN, sessionID, buffer, len);
			};
		GPacketHandlerFunc[PKT_C_ENTER_ROOM] = [](int32 sessionID, unsigned char* buffer, int32 len)
			{
				return HandlePacket <Protocol::C_ENTER_ROOM>(Handle_C_ENTER_ROOM, sessionID, buffer, len);
			};
		GPacketHandlerFunc[PKT_C_LEAVE_ROOM] = [](int32 sessionID, unsigned char* buffer, int32 len)
			{
				return HandlePacket <Protocol::C_LEAVE_ROOM>(Handle_C_LEAVE_ROOM, sessionID, buffer, len);
			};
		GPacketHandlerFunc[PKT_C_MOVE] = [](int32 sessionID, unsigned char* buffer, int32 len)
			{
				return HandlePacket <Protocol::C_MOVE>(Handle_C_MOVE, sessionID, buffer, len);
			};
		GPacketHandlerFunc[PKT_C_CHAT] = [](int32 sessionID, unsigned char* buffer, int32 len)
			{
				return HandlePacket <Protocol::C_CHAT>(Handle_C_CHAT, sessionID, buffer, len);
			};
		GPacketHandlerFunc[PKT_C_DISCONNECT] = [](int32 sessionID, unsigned char* buffer, int32 len)
			{
				return HandlePacket <Protocol::C_DISCONNECT>(Handle_C_DISCONNECT, sessionID, buffer, len);
			};
	}

	static bool HandlePacket(int32 sessionID, unsigned char* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandlerFunc[header->Packet_Id](sessionID, buffer, len);
	}
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_LOGIN&packet) { return MakeSendBuffer(packet, PKT_S_LOGIN); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_ENTER_ROOM&packet) { return MakeSendBuffer(packet, PKT_S_ENTER_ROOM); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_SPAWN&packet) { return MakeSendBuffer(packet, PKT_S_SPAWN); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_DESPAWN&packet) { return MakeSendBuffer(packet, PKT_S_DESPAWN); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_MOVE&packet) { return MakeSendBuffer(packet, PKT_S_MOVE); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_CHAT&packet) { return MakeSendBuffer(packet, PKT_S_CHAT); }
	

private:
	template<typename PacketType, typename ProcessFunc>
	static bool HandlePacket(ProcessFunc func, int32 sessionID, unsigned char* buffer, int32 len)
	{
		PacketType pkt;
		if (pkt.ParseFromArray(buffer + sizeof(PacketHeader), len - sizeof(PacketHeader)) == false)
		{
			#if UE_BUILD_DEBUG + UE_BUILD_DEVELOPMENT + UE_BUILD_TEST + UE_BUILD_SHIPPING >= 1

			#else
			cout << "pkt.ParseFromArray fail" << "\n";
			#endif
			return false;
		}
		return func(sessionID, pkt);
	}

	template<typename T>
	static shared_ptr<SendBuffer> MakeSendBuffer(T& pkt, uint16 pktID)
	{
		shared_ptr<SendBuffer> sendBuffer = make_shared<SendBuffer>(4096);
		// header는 사실상 sendBuffer의 m_Buffer의 시작 주소
		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->GetBuffer());
		uint16 headersize = sizeof(PacketHeader);
		header->Packet_Id = pktID;
		header->PacketSize = static_cast<uint16>(pkt.ByteSizeLong());

		
		// 패킷 직렬화해서 sendBuffer에 header파일 이어서 넣어주기
		pkt.SerializeToArray(&header[1], header->PacketSize);


		// 즉, 여기까지 데이터 밀어넣은 과정은. CopyData 한거랑 똑같음.
		sendBuffer->Write(headersize + header->PacketSize);
		return sendBuffer;
	}
};