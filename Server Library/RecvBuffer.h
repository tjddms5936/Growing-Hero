#pragma once

// �ϳ��� ���۸� ������ ��Ȱ�� �ϱ� ����
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
	int32 m_Capacity; // ������ ���� �� ���� ũ��
	int32 m_BufferSize; // �� ��� ���� ũ��
	int32 m_readPos; // ���� read ��ġ
	int32 m_writePos; // ���� write ��ġ
	int32 m_BufferScale = 10; // ���� ��� ������ ����

	vector<unsigned char> m_RecvBuffer;

};

