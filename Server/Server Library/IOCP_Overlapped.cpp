#include "pch.h"
#include "IOCP_Overlapped.h"

void IOCP_Overlapped::SetOwnerObject(shared_ptr<IOCP_Object> OwnerObject)
{
	m_OwnerObject = OwnerObject;
}

void IOCP_Overlapped::SetManagedObject(shared_ptr<IOCP_Session> ManagedSession)
{
	m_ManagedSession = ManagedSession;
}

void IOCP_Overlapped::init()
{
	OVERLAPPED::hEvent = 0;
	OVERLAPPED::Internal = 0;
	OVERLAPPED::InternalHigh = 0;
	OVERLAPPED::Offset = 0;
	OVERLAPPED::OffsetHigh = 0;
}
