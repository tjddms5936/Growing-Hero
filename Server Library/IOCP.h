#pragma once

class IOCP_Object;

class IOCP
{
public:
	IOCP() = default;
	IOCP(uint32 MaxIocp_ID);
	~IOCP();

public:
	void init();
	HANDLE GetHandle(){ return m_IocpHandle; }

	bool RegisterToCP(shared_ptr<IOCP_Object> Object);
	void ReleaseFromCP(uint32 ObjectID);
	bool WorkSearch();

	uint16 GetNewIOCP_ID();

private:
	HANDLE m_IocpHandle;
	vector<shared_ptr<IOCP_Object>> m_RegisteredObjects;
	std::queue<uint16> m_EnableNewIocp_Object_ID;
	uint32 m_MaxIocp_ID_Cnt;

	std::mutex m;
};

