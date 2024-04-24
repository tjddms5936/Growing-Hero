#include "ServerPacketHandler.h"
#include "Protocol.pb.h"
#include "MainGameInstance.h"

PacketHandlerFunc GPacketHandlerFunc[UINT16_MAX];
UMainGameInstance* GGameInstance = nullptr;

bool Handle_INVALID(int32 sessionID, unsigned char* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

	return true;
}

bool Handle_S_LOGIN(int32 sessionID, Protocol::S_LOGIN& pkt)
{
	if (UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->Work_S_LOGIN(pkt);
	}
	return true;
}

bool Handle_S_ENTER_ROOM(int32 sessionID, Protocol::S_ENTER_ROOM& pkt)
{
	if (UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->Work_S_ENTER_ROOM(pkt);
	}
	return true;
}

bool Handle_S_SPAWN(int32 sessionID, Protocol::S_SPAWN& pkt)
{
	if (UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->Work_S_SPAWN(pkt);
	}
	return true;
}

bool Handle_S_DESPAWN(int32 sessionID, Protocol::S_DESPAWN& pkt)
{
	if (UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->Work_S_DESPAWN(pkt);
	}
	return true;
}

bool Handle_S_MOVE(int32 sessionID, Protocol::S_MOVE& pkt)
{
	if (UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->Work_S_MOVE(pkt);
	}
	return true;
}

bool Handle_S_CHAT(int32 sessionID, Protocol::S_CHAT& pkt)
{
	if (UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->Work_S_CHAT(pkt);
	}
	return true;
}
