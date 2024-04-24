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


	// ���������� ����� Session ������. AcceptOverlapped�� ����.
	shared_ptr<IOCP_Session> GetManagedSession() { return m_ManagedSession; }
	// ���������� ����� Session ������. AcceptOverlapped�� ����.
	void SetManagedObject(shared_ptr<IOCP_Session> ManagedSession);

	void init();


public:
	// send�� ���۵� ���� ������ ���ر��� ��Ƴ��� ����.
	vector<shared_ptr<SendBuffer>> m_vSendBuffer;


private:
	OverlapType m_OverlapType;
	shared_ptr<IOCP_Object> m_OwnerObject;
	shared_ptr<IOCP_Session> m_ManagedSession;
	
};

