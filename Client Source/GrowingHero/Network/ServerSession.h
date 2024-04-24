// Fill out your copyright notice in the Description page of Project Settings.

#pragma once



#include "CoreMinimal.h"
#include "mutex"

class GROWINGHERO_API ServerSession : public TSharedFromThis<ServerSession>
{
public:
	// SessionID는 서버 접속 성공시 서버로부터 부여되는 SessionID. 
	ServerSession(class FSocket* Socket);
	~ServerSession();
	
public:
	void init();
	void Release();

	void SendToServer(TSharedPtr<class SendBuffer> sendbuffer);

	int16 GetSessionID() { return m_SessionID; }
	void SetSessionID(int16 ID) { m_SessionID = ID; }

	void MonitorRecvPackets();
public:
	class FSocket* m_Socket;
	TSharedPtr<class RecvThread> m_RecvThread;
	TSharedPtr<class SendThread> m_SendThread;

	// GameThread와 NetworkThread가 데이터 주고 받는 공용 큐.
	TQueue<TArray<uint8>> m_RecvPacketQueue;
	TQueue<TSharedPtr<SendBuffer>> m_SendPacketQueue;

	std::mutex m;

	int32 m_RecvDataQSize;
private:
	int16 m_SessionID;
};
