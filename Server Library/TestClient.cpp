#include "pch.h"
#include "TestClient.h"
#include "SessionMGR.h"
#include "IOCP.h"

void TestClient::init(wstring ip, uint16 port, int32 maxSessionCount)
{
    m_maxSessionCount = maxSessionCount;
    
    

    m_addr;
    memset(&m_addr, 0, sizeof(m_addr));
    m_addr.sin_family = AF_INET;

    IN_ADDR address;
    ::InetPtonW(AF_INET, ip.c_str(), &address);

    m_addr.sin_addr = address;
    m_addr.sin_port = htons(port);
}

bool TestClient::Start()
{

    for (int i = 0; i < m_maxSessionCount; i++)
    {
        shared_ptr<IOCP_Session> session = GSessionMGR->CreateSession();
        session->SetServiceType(ServiceType::CLIENT);
        session->SetNetworkAddr(m_addr);
        

        if (session->RegisterConnect() == false)
            return false;
        m_vSessions.push_back(session);
    }

    return true;
}

HANDLE TestClient::GetHandle()
{
    return nullptr;
}

void TestClient::CP_Work(IOCP_Overlapped* pOverlapped, int32 DataSize)
{

}