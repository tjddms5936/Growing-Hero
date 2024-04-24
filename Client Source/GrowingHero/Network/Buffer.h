// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


class GROWINGHERO_API SendBuffer : public TSharedFromThis<SendBuffer>
{
public:
	SendBuffer(int32 bufferSize = 4096);
	~SendBuffer() = default;
	SendBuffer(const SendBuffer&) = delete;
	SendBuffer& operator=(const SendBuffer&) = delete;

public:
	unsigned char* GetBuffer() { return m_sendBuffer.GetData(); }
	int32 GetBufferCapacity() { return m_BufferCapacity; }
	int32 GetWriteSize() { return m_writeSize; }
	void Write(int32 writeSize) { m_writeSize += writeSize; }

private:
	TArray<BYTE> m_sendBuffer;
	int32 m_BufferCapacity; // 총 버퍼 크기
	int32 m_writeSize; // 실 사용 크기
};

