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
		// ���� �����Ͱ� �ʹ� ũ��... ©�� ������ �ϳ�? �ϴ� �������� ���߿� ����
		CRASH();
		return false;
	}

	// PacketHeader ����ü ���� �ϰ� ���� ������ ��.
	// ���� m_sendBuffer�� �Ǿտ� �ٿ��� �����ֱ�
	PacketHeader* Header = reinterpret_cast<PacketHeader*>(m_sendBuffer.data());

	// Header�� Header������� ������ ������ ����
	uint16 headerSize = static_cast<uint16>(sizeof(PacketHeader));
	Header->PacketSize = static_cast<uint16>(Datalen) + headerSize;
	Header->Packet_Id = PacketID;

	// ������ ���� ������ Header �ڷ� �ٿ��� ����
	::memcpy(&m_sendBuffer[headerSize], SendData, Datalen);

	// �� ����� ������ ����
	m_writeSize += headerSize + Header->PacketSize;
	return true;
}
