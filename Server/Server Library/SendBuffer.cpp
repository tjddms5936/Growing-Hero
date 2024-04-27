#include "pch.h"
#include "SendBuffer.h"

SendBuffer::SendBuffer(int32 bufferSize) :
	m_BufferCapacity(bufferSize), m_writeSize{}
{
	m_sendBuffer.resize(m_BufferCapacity);
}

bool SendBuffer::DataWrite(int32 PacketID, void* SendData, int32 Datalen)
{
	if (Datalen > m_BufferCapacity)
	{
		// 보낼 데이터가 너무 크면... 짤라서 보내야 하나? 일단 에러띄우고 나중에 생각
		CRASH();
		return false;
	}

	// PacketHeader 구조체 정의 하고 같이 보내야 함.
	// 보낼 m_sendBuffer에 맨앞에 붙여서 보내주기
	PacketHeader* Header = reinterpret_cast<PacketHeader*>(m_sendBuffer.data());

	// Header에 Header사이즈랑 데이터 사이즈 구분
	uint16 headerSize = static_cast<uint16>(sizeof(PacketHeader));
	Header->PacketSize = static_cast<uint16>(Datalen) + headerSize;
	Header->Packet_Id = PacketID;

	// 나머지 보낼 데이터 Header 뒤로 붙여서 복사
	::memcpy(&m_sendBuffer[headerSize], SendData, Datalen);

	// 총 사용한 사이즈 저장
	m_writeSize += headerSize + Header->PacketSize;
	return true;
}
