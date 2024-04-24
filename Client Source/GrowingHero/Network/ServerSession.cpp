// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/ServerSession.h"
#include "NetworkThread.h"
#include "ServerPacketHandler.h"

ServerSession::ServerSession(class FSocket* Socket) :
	m_Socket(Socket), m_RecvDataQSize{}, m_SessionID{}
{
	ServerPacketHandler::init();
	
}

ServerSession::~ServerSession()
{
	Release();
}

void ServerSession::init()
{
	UE_LOG(LogTemp, Warning, TEXT("ServerSession::Init()"));

	m_RecvThread = MakeShared<RecvThread>(m_Socket, AsShared());
	m_SendThread = MakeShared<SendThread>(m_Socket, AsShared());

}

void ServerSession::Release()
{
	if (m_RecvThread)
	{
		m_RecvThread->Stop();
		m_RecvThread = nullptr;
	}

	if (m_SendThread)
	{
		m_SendThread->Stop();
		m_SendThread = nullptr;
	}
	
	m_RecvPacketQueue.Empty();
	m_SendPacketQueue.Empty();
	m_Socket = nullptr;
}

void ServerSession::SendToServer(TSharedPtr<SendBuffer> sendbuffer)
{
	m_SendPacketQueue.Enqueue(sendbuffer);
}

void ServerSession::MonitorRecvPackets()
{
	while (true)
	{
		TArray<uint8> Packet;
		if (m_RecvPacketQueue.Dequeue(OUT Packet) == false)
			break;
		m_RecvDataQSize--;

		TSharedPtr<ServerSession> ThisPtr = AsShared();
		if(ThisPtr.IsValid())
			ServerPacketHandler::HandlePacket(GetSessionID(), Packet.GetData(), Packet.Num());
	}
}
