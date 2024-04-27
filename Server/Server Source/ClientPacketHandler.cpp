#include "pch.h"
#include "ClientPacketHandler.h"
#include "PacketProcessor.h"

PacketHandlerFunc GPacketHandlerFunc[UINT16_MAX];

bool Handle_INVALID(int32 sessionID, unsigned char* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	// TODO : Log
	return false;
}

bool Handle_C_LOGIN(int32 sessionID, Protocol::C_LOGIN& pkt)
{
	GPktProcessor.Work_C_LOGIN(sessionID, pkt);
	return true;
}


bool Handle_C_ENTER_ROOM(int32 sessionID, Protocol::C_ENTER_ROOM& pkt)
{
	GPktProcessor.Work_C_ENTER_ROOM(sessionID, pkt);
	return true;
}

bool Handle_C_LEAVE_ROOM(int32 sessionID, Protocol::C_LEAVE_ROOM& pkt)
{
	GPktProcessor.Work_C_LEAVE_ROOM(sessionID, pkt);
	return true;
}

bool Handle_C_MOVE(int32 sessionID, Protocol::C_MOVE& pkt)
{
	GPktProcessor.Work_C_MOVE(pkt);
	return true;
}

bool Handle_C_CHAT(int32 sessionID, Protocol::C_CHAT& pkt)
{
	GPktProcessor.Work_C_CHAT(pkt);
	return true;
}

bool Handle_C_DISCONNECT(int32 sessionID, Protocol::C_DISCONNECT& pkt)
{

	return true;
}