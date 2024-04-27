#pragma once
#include "SendBuffer.h"

enum OverlapType
{
	Connect,
	DisConnect,
	Accept,
	Recv,
	Send
};

class IOCP_Object;
class IOCP_Session;

class IOCP_Overlapped : public OVERLAPPED
{

public:
	OverlapType GetOverlapType() { return m_OverlapType; }
	void SetOverlapType(OverlapType eOvelapType) { m_OverlapType = eOvelapType; }

	shared_ptr<IOCP_Object> GetOwnerObject() { return m_OwnerObject; }
	void SetOwnerObject(shared_ptr<IOCP_Object> OwnerObject);


	// 서버측에서 연결된 Session 관리용. AcceptOverlapped을 위함.
	shared_ptr<IOCP_Session> GetManagedSession() { return m_ManagedSession; }
	// 서버측에서 연결된 Session 관리용. AcceptOverlapped을 위함.
	void SetManagedObject(shared_ptr<IOCP_Session> ManagedSession);

	void init();


public:
	// send할 버퍼들 적정 사이즈 수준까지 담아놓기 위함.
	vector<shared_ptr<SendBuffer>> m_vSendBuffer;


private:
	OverlapType m_OverlapType;
	shared_ptr<IOCP_Object> m_OwnerObject;
	shared_ptr<IOCP_Session> m_ManagedSession;
	
};

