#include "pch.h"
#include "SessionMGR.h"
#include "IOCP_Object.h"
#include "IOCP.h"

SessionMGR::SessionMGR(uint32 MaxSessionCnt) :
	m_ConnectedSessions(MaxSessionCnt, nullptr), m_MaxSessionCnt(MaxSessionCnt)
{
	
}

void SessionMGR::init(SessionMaker factory)
{
	m_sessionFactory = factory;
	// 등록 가능한 Session 고유 ID를 미리 확보
	for (uint16 i = 1; i <= m_MaxSessionCnt; ++i)
	{
		m_EnableNewSessionID.push(i);
	}
}

shared_ptr<IOCP_Session> SessionMGR::CreateSession()
{
	shared_ptr<IOCP_Session> NewSession = m_sessionFactory();
	GIOCP->RegisterToCP(NewSession->shared_from_this());
	return NewSession;
}

bool SessionMGR::AddSession(shared_ptr<IOCP_Session> SessionRef)
{
	SessionRef->SetSessionID(m_EnableNewSessionID.front());
	m_EnableNewSessionID.pop();

	if (SessionRef->GetSessionID() >= m_MaxSessionCnt || SessionRef->GetSessionID() < 0)
	{
		CRASH();
		return false;
	}
	if (m_ConnectedSessions[SessionRef->GetSessionID()] != nullptr)
	{
		// 다른 Session이 정상 해제 되지 않았거나 애초에 등록이나 초기화가 잘못됨
		CRASH();
		return false;
	}

	m_ConnectedSessions[SessionRef->GetSessionID()] = SessionRef;
	return true;
}

void SessionMGR::ReleaseSession(uint32 SessionID)
{
	if (SessionID >= m_MaxSessionCnt || m_MaxSessionCnt < 0)
	{
		CRASH();
		return;
	}

	m_ConnectedSessions[SessionID] = nullptr;
	m_EnableNewSessionID.push(SessionID);
}

uint16 SessionMGR::GetNewSessionID()
{
	uint16 ret = m_EnableNewSessionID.front();
	m_EnableNewSessionID.pop();
	return ret;
}
