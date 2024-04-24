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
	// �� ������ �����ϴ� Plyer ������ �ʿ�
	// OnDisconnect����... Player���� Room���� �̾Ƽ�
	// Room->Release(PlayerID) ����� ��.

	// �ƴѰ�? ��¥�� Room�� �˰������� �ش� Room���� �� SessionID�� �� PlayerID�̴ϱ�
	// Room->ReleasePlayer(GetSessionID) ���ָ� �ɵ�? 
	// �׷��� �׳� �� ������ ������ SessionID�� �´� Session���� ���� Room������ ������Ʈ ������
	atomic<Protocol::RoomType> m_Room_PlayerdIn;
};

