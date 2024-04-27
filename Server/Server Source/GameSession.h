#pragma once
#include "IOCP_Object.h"
#include "UnitBase.h"

class GameSession : public IOCP_Session
{
public:
	GameSession();
	~GameSession();

public:
	virtual void OnConnect() override;
	virtual void OnDisconnect() override;
	virtual bool OnRecv(unsigned char* recvBuffer, int32 len) override;
	virtual void OnSend(int32 len);

	void SetPlayerRoomType(Protocol::RoomType type) { m_Room_PlayerdIn.store(type); }
private:
	// 이 세션이 관리하는 Plyer 정보가 필요
	// OnDisconnect에서... Player현재 Room정보 뽑아서
	// Room->Release(PlayerID) 해줘야 함.

	// 아닌가? 어짜피 Room만 알고있으면 해당 Room에서 내 SessionID가 곧 PlayerID이니까
	// Room->ReleasePlayer(GetSessionID) 해주면 될듯? 
	// 그러면 그냥 방 입장할 때마다 SessionID에 맞는 Session으로 가서 Room정보만 업데이트 해주자
	atomic<Protocol::RoomType> m_Room_PlayerdIn;
};

