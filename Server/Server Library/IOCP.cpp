#include "pch.h"
#include "IOCP.h"
#include "IOCP_Object.h"
#include "IOCP_Overlapped.h"

IOCP::IOCP(uint32 MaxIocp_ID) :
    m_IocpHandle{}, m_MaxIocp_ID_Cnt(MaxIocp_ID)
{
    m_IocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
    if (m_IocpHandle == INVALID_HANDLE_VALUE)
        CRASH();
}

IOCP::~IOCP()
{
    ::CloseHandle(m_IocpHandle);
}

void IOCP::init()
{
    m_RegisteredObjects.resize(m_MaxIocp_ID_Cnt+4);
    fill(m_RegisteredObjects.begin(), m_RegisteredObjects.end(), nullptr);

    for (uint16 i=1; i<= m_MaxIocp_ID_Cnt; i++)
    {
        m_EnableNewIocp_Object_ID.push(i);
    }
}

bool IOCP::RegisterToCP(shared_ptr<IOCP_Object> Object)
{
    lock_guard<mutex> guard(m);
    HANDLE retHandle =
        ::CreateIoCompletionPort(Object->GetHandle(), m_IocpHandle, 0, 0);

    if (retHandle == NULL)
        return false;
    
    // IOCP_Object의 고유 ID 부여
    Object->SetOwnObjectID(m_EnableNewIocp_Object_ID.front());
    m_EnableNewIocp_Object_ID.pop();

    // 등록된 Object를 고유 ID에 해당하는 인덱스 자리에 넣어줌
   m_RegisteredObjects[Object->GetOwnObjectID()] = Object;
   return true;
}

void IOCP::ReleaseFromCP(uint32 ObjectID)
{
    lock_guard<mutex> guard(m);
    // 등록되었던 자리 비어줌. 
    m_RegisteredObjects[ObjectID] = nullptr;
    m_EnableNewIocp_Object_ID.push(ObjectID);
}

bool IOCP::WorkSearch()
{
    DWORD numberOfBytesTransffered{};
    ULONG_PTR CompletionKey{};
    IOCP_Overlapped* ResultsOverlapped{};
    BOOL ret = ::GetQueuedCompletionStatus(m_IocpHandle, &numberOfBytesTransffered, &CompletionKey, 
        reinterpret_cast<LPOVERLAPPED*>(&ResultsOverlapped), INFINITE);

    if (ret == TRUE)
    {
        ResultsOverlapped->GetOwnerObject()->CP_Work(ResultsOverlapped, numberOfBytesTransffered);
    }
    else
    {
        int32 errorCode = ::WSAGetLastError();
        switch (errorCode)
        {
        case WAIT_TIMEOUT:
            return false;
        default:
            ResultsOverlapped->GetOwnerObject()->CP_Work(ResultsOverlapped, numberOfBytesTransffered);
            break;
        }
    }

    return true;
}

uint16 IOCP::GetNewIOCP_ID()
{
    uint16 ret = m_EnableNewIocp_Object_ID.front();
    m_EnableNewIocp_Object_ID.pop();
    return ret;
}
