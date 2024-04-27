#pragma once

// 버퍼 쓰기, 복사 기능. Session쪽에서 보낼 데이터들 이걸로 쓴다음에
// 큐에 계속 넣어줄거임. 한번에 다량의 SendBuffer 처리 작업 필요
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
	int32 m_BufferCapacity; // 총 버퍼 크기
	int32 m_writeSize; // 실 사용 크기
};

