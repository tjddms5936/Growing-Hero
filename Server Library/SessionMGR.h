#pragma once
#include "IOCP_Object.h"


class SessionMGR
{
public:
	SessionMGR() = default;
	SessionMGR(uint32 MaxSessionCnt = 10000);
	~SessionMGR() = default;
	SessionMGR(const SessionMGR&) = delete;
	SessionMGR& operator=(const SessionMGR&) = delete;

public:
	void init(SessionMaker factory);
	shared_ptr<IOCP_Session> CreateSession();
	bool AddSession(shared_ptr<IOCP_Session> SessionRef);
	void ReleaseSession(uint32 SessionID);

	uint16 GetNewSessionID();

private:


private:
	vector<shared_ptr<IOCP_Session>> m_ConnectedSessions;
	uint32 m_MaxSessionCnt;

	std::queue<uint16> m_EnableNewSessionID;

	SessionMaker m_sessionFactory;
};

