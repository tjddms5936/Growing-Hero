// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkThread.h"
#include "Buffer.h"

#include "Sockets.h"
#include "Serialization/ArrayWriter.h"
#include "ServerSession.h"
#include "ServerPacketHandler.h"

// ============================= Recv Thread =============================

RecvThread::RecvThread(FSocket* Socket, TSharedPtr<ServerSession> Session) :
	m_OwnerSession(Session), m_RecvThread{}, bLoop{}, m_Socket(Socket)
{
	bLoop = true;
	m_RecvThread = FRunnableThread::Create(this, TEXT("RecvThread"));
}

RecvThread::~RecvThread()
{
	// m_RecvThread->Kill();
}

bool RecvThread::Init()
{
	// TODO
	UE_LOG(LogTemp, Warning, TEXT("RecvThread::Init()"));
	return true;
}

uint32 RecvThread::Run()
{
	headerSize = sizeof(FPacketHeader);
	while (bLoop)
	{
		TArray<uint8> Packet;
		if (RecvPacket(Packet))
		{
			static int32 PrintCnt = 0;
			// 정상 수신 완료
			if (TSharedPtr<ServerSession> Session = m_OwnerSession.Pin())
			{
				Session->m_RecvPacketQueue.Enqueue(Packet);
				Session->m_RecvDataQSize++;
			}
		}

	}
	return 0;
}

void RecvThread::Exit()
{
	// TODO
	return;
}

void RecvThread::Stop()
{
	bLoop = false;
}

bool RecvThread::RecvPacket(TArray<uint8>& Packet)
{
	// 패킷 헤더와 페이로드 부분 구분 파싱

	// 패킷 먼저 헤더 파싱
	TArray<uint8> header;
	header.AddZeroed(headerSize);

	if (RecvSpecificSize(header.GetData(), headerSize) == false)
		return false;

	

	FPacketHeader PktHeader{};
	{
		FMemoryReader Reader(header);
		Reader << PktHeader;
	}


	// 패킷 헤더 복사
	Packet = header;

	// 패킷 내용 추출
	TArray<uint8> Payload;
	const int32 PayloadSize = PktHeader.PacketSize;
	if (PayloadSize <= 0)
	{
		return true;
	}

	Packet.AddZeroed(PayloadSize);

	if (RecvSpecificSize(&Packet[headerSize], PayloadSize) == false)
		return false;

	return true;
}

bool RecvThread::RecvSpecificSize(uint8* buffer, int32 size)
{
	// size만큼만 추출해서 buffer에 저장해주기
	
	// 블로킹하지않고 데이터 유무 판단
	uint32 PendingDataSize{};
	if (m_Socket->HasPendingData(PendingDataSize) == false || PendingDataSize <= 0)
		return false;

	// 이제 무조건 데이터 소켓에서 수신 대기중인 데이터가 있음
	
	int32 BufferPos = 0;
	while (size > 0)
	{
		int32 recvSize{};
		m_Socket->Recv(buffer + BufferPos, size, recvSize);
		check(recvSize <= size);

		if (recvSize <= 0)
		{
			// 연결 끊김 처리?
			return false;
		}

		BufferPos += recvSize;
		size -= recvSize;
	}

	return true;
}

// ============================= Send Thread =============================

SendThread::SendThread(FSocket* Socket, TSharedPtr<ServerSession> Session) :
	m_OwnerSession(Session), m_SendThread{}, bLoop{}, m_Socket(Socket)
{
	bLoop = true;
	m_SendThread = FRunnableThread::Create(this, TEXT("SendThread"));
}

SendThread::~SendThread()
{
	m_SendThread->Kill();
}

bool SendThread::Init()
{
	// TODO
	UE_LOG(LogTemp, Warning, TEXT("SendThread::Init()"));
	return true;
}

uint32 SendThread::Run()
{
	while (bLoop)
	{
		TSharedPtr<SendBuffer> sendBuffer;
		TSharedPtr<ServerSession> Session = m_OwnerSession.Pin();
		if (Session && Session->m_SendPacketQueue.Dequeue(sendBuffer) == true)
		{
			SendPacket(sendBuffer);
		}
	}
	return 0;
}

void SendThread::Exit()
{
	// TODO 
	return;
}

void SendThread::Stop()
{
	bLoop = false;
}

bool SendThread::SendPacket(TSharedPtr<SendBuffer> sendBuffr)
{
	// sendBuffr는 채워져서 왔다. 즉, 얼마나 쓴지 확인 가능
	int32 SendRemainedSize = sendBuffr->GetWriteSize();
	BYTE* sendData = sendBuffr->GetBuffer();

	while (SendRemainedSize > 0)
	{
		int32 sentSize{};
		if (m_Socket->Send(sendData, SendRemainedSize, sentSize) == false)
		{
			// 연결 끊겼다고 봐도 되려나?
			return false;
		}

		SendRemainedSize -= sentSize;
		sendData += sentSize;
	}
	return true;
}
