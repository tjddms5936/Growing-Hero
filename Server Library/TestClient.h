#pragma once
#include <functional>
#include "IOCP_Object.h"


class TestClient : IOCP_Object
{
public:
	TestClient() = default;
	~TestClient() = default;

public:
	virtual HANDLE GetHandle() override;
	virtual void CP_Work(IOCP_Overlapped* pOverlapped, int32 DataSize = 0) override;

public:
	void	init(wstring ip, uint16 port, int32 maxSessionCount = 1);
	bool	Start();

private:
	int32				m_maxSessionCount = 0;
	SOCKADDR_IN			m_addr;

public:
	vector<shared_ptr<IOCP_Session>> m_vSessions;
};

