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

	// ���� ��� : ������ȭ�ؼ� ����ü ���� ä���
	friend FArchive& operator<<(FArchive& Ar, FPacketHeader& Header)
	{
		Ar << Header.PacketSize;
		Ar << Header.Packet_Id;
		return Ar;
	}

	// pktSize�� ��������� ������ Payload �������̴�.
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
	// Ŭ��� �ϳ��� Session�� �ְ� Session�� �ϳ��� RecvThread�� �ִ�.  
	// ��ȯ ���� ����
	TWeakPtr<ServerSession> m_OwnerSession;
	FRunnableThread* m_RecvThread;
	bool bLoop;
	FSocket* m_Socket;

	// �����κ��� �޴°Ŵ� ���� ť�� �־��ش� �ϴ�.
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
