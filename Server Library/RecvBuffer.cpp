#include "pch.h"
#include "RecvBuffer.h"

RecvBuffer::RecvBuffer(int32 bufferSize) : 
	m_Capacity{}, m_BufferSize(bufferSize), m_readPos(0), m_writePos(0)
{
	m_Capacity = m_BufferSize * m_BufferScale;
	m_RecvBuffer.resize(m_Capacity);
}

void RecvBuffer::Reset()
{
	m_readPos = 0;
	m_writePos = 0;
}

void RecvBuffer::CopyToFront()
{
	int32 dataSize = m_writePos - m_readPos;
	// 여유 공간이 buffersize 1개 미만이면 앞으로 그대로 복사
	::memcpy(&m_RecvBuffer[0], &m_RecvBuffer[m_readPos], dataSize);
	m_readPos = 0;
	m_writePos = dataSize;
}

bool RecvBuffer::ReadComplete(int32 ReadSize)
{
	int32 dataSize = m_writePos - m_readPos;
	if (ReadSize > dataSize)
		return false;
	
	m_readPos += dataSize;
	return true;
}

bool RecvBuffer::WriteComplete(int32 WriteSize)
{
	int32 dataSize = WriteSize - m_readPos;
	int32 RemainedSize = m_Capacity - m_writePos;

	if (dataSize > RemainedSize)
		return false;

	m_writePos += WriteSize;
	return true;
}
