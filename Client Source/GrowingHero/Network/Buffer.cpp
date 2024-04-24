// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/Buffer.h"

SendBuffer::SendBuffer(int32 bufferSize) :
	m_BufferCapacity(bufferSize), m_writeSize{}
{
	m_sendBuffer.SetNum(m_BufferCapacity);
}

