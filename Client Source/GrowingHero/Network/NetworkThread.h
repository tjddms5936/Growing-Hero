// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "mutex"

struct FPacketHeader
{
public:
	FPacketHeader() : PacketSize(0), Packet_Id(0)
	{
	}
	FPacketHeader(uint16 packetSize, uint16 packetID) : PacketSize(packetSize), Packet_Id(packetID)
	{
	}

	// 엔진 기능 : 역직렬화해서 구조체 내용 채우기
	friend FArchive& operator<<(FArchive& Ar, FPacketHeader& Header)
	{
		Ar << Header.PacketSize;
		Ar << Header.Packet_Id;
		return Ar;
	}

	// pktSize는 헤더파일을 제외한 Payload 사이즈이다.
	uint16 PacketSize;
	uint16 Packet_Id;
};

class RecvThread : public FRunnable
{
public:
	RecvThread(FSocket* Socket, TSharedPtr<class ServerSession> Session);
	~RecvThread();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;

	void Stop();
private:
	bool RecvPacket(TArray<uint8>& Packet);
	bool RecvSpecificSize(uint8* buffer, int32 size);

private:
	// 클라당 하나의 Session이 있고 Session은 하나의 RecvThread가 있다.  
	// 순환 참조 방지
	TWeakPtr<ServerSession> m_OwnerSession;
	FRunnableThread* m_RecvThread;
	bool bLoop;
	FSocket* m_Socket;

	// 서버로부터 받는거는 전부 큐에 넣어준다 일단.
	std::mutex m;
	uint32 headerSize;
};

class SendThread : public FRunnable
{
public:
	SendThread(FSocket* Socket, TSharedPtr<class ServerSession> Session);
	~SendThread();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;

	void Stop();
public:
	bool SendPacket(TSharedPtr<SendBuffer> sendBuffr);

private:
	TWeakPtr<ServerSession> m_OwnerSession;
	FRunnableThread* m_SendThread;
	bool bLoop;
	FSocket* m_Socket;

	std::mutex m;
};
