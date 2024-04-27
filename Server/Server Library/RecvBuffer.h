#pragma once

// 하나의 버퍼를 가지고 재활용 하기 위함
class RecvBuffer
{
public:
	RecvBuffer(int32 bufferSize);
	~RecvBuffer() = default;
	RecvBuffer(const RecvBuffer&) = delete;
	RecvBuffer& operator =(const RecvBuffer&) = delete;

public:
	void Reset();
	void CopyToFront();
	bool ReadComplete(int32 ReadSize);
	bool WriteComplete(int32 WriteSize);

	unsigned char* GetCurReadPos() { return &m_RecvBuffer[m_readPos]; }
	unsigned char* GetCurWritePos() { return &m_RecvBuffer[m_writePos]; }
	ULONG GetRemainedSize() { return m_Capacity - m_writePos; }
	int32 GetCurDataSize() { return m_writePos - m_readPos; }

private:
	int32 m_Capacity; // 여유분 포함 총 버퍼 크기
	int32 m_BufferSize; // 실 사용 버퍼 크기
	int32 m_readPos; // 현재 read 위치
	int32 m_writePos; // 현재 write 위치
	int32 m_BufferScale = 10; // 실제 사용 버퍼의 배율

	vector<unsigned char> m_RecvBuffer;

};

