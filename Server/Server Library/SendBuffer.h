#pragma once

// ���� ����, ���� ���. Session�ʿ��� ���� �����͵� �̰ɷ� ��������
// ť�� ��� �־��ٰ���. �ѹ��� �ٷ��� SendBuffer ó�� �۾� �ʿ�
class SendBuffer : public enable_shared_from_this<SendBuffer>
{
public:
	SendBuffer(int32 bufferSize = 4096);
	~SendBuffer() = default;
	SendBuffer(const SendBuffer&) = delete;
	SendBuffer& operator=(const SendBuffer&) = delete;

public:
	unsigned char* GetBuffer() { return m_sendBuffer.data(); }
	int32 GetBufferCapacity() { return m_BufferCapacity; }
	int32 GetWriteSize() { return m_writeSize; }
	void Write(int32 writeSize) { m_writeSize += writeSize; }

	bool DataWrite(int32 PacketID, void* SendData, int32 Datalen);

private:
	vector<unsigned char> m_sendBuffer;
	int32 m_BufferCapacity; // �� ���� ũ��
	int32 m_writeSize; // �� ��� ũ��
};

