#include "pch.h"
#include "IOCP_Object.h"
#include "IOCP_Overlapped.h"
#include "SocketMGR.h"
#include "SessionMGR.h"
#include "IOCP.h"

// ===================================== IOCP_Listener =======================

IOCP_Listener::~IOCP_Listener()
{
	SocketMGR::CloseSocket(m_ListenerSocket);
	for (IOCP_Overlapped* acceptStr : m_arAcceptOverlapped)
	{
		delete(acceptStr);
	}
}

HANDLE IOCP_Listener::GetHandle()
{
	return reinterpret_cast<HANDLE>(m_ListenerSocket);
}

void IOCP_Listener::CP_Work(IOCP_Overlapped* pOverlapped, int32 DataSize)
{
	if (pOverlapped->GetOverlapType() != OverlapType::Accept)
	{
		cout << "IOCP_Listener::CP_Work Error" << "\n";
		CRASH();
		return;
	}
	else
	{
		Process_IOCP_Accept(pOverlapped);
	}
	return;
}


void IOCP_Listener::init(SessionMaker factory)
{
	// IOCP�� SessionMGR ���� init ���ֱ�. �׷��� ���� ��� ����
	GIOCP->init();
	GSessionMGR->init(factory);

	// overlapped �迭�� �񵿱� �Լ� ���� �Ϸ�� ä�� ��ȯ
	m_ListenerSocket = SocketMGR::CreateSocket();
	if (m_ListenerSocket == INVALID_SOCKET)
	{
		CRASH();
		return;
	}

	SocketMGR::SetSocketOption(m_ListenerSocket, OptionName::X_LINGER, 0);
	SocketMGR::SetSocketOption(m_ListenerSocket, OptionName::X_REUSEADDR, true);
}

bool IOCP_Listener::StartListen(wstring IP, uint16 Port)
{
	if (m_ListenerSocket == INVALID_SOCKET)
	{
		CRASH();
		return false;
	}
	
	// ��Ʈ��ũ ����
	memset(&m_SocketAddress, 0, sizeof(m_SocketAddress));
	m_SocketAddress.sin_family = AF_INET;
	m_SocketAddress.sin_addr = IP_to_Addr(IP.c_str());
	m_SocketAddress.sin_port = htons(Port);

	// ���ε�
	if (SOCKET_ERROR == ::bind(m_ListenerSocket, reinterpret_cast<const SOCKADDR*>(&m_SocketAddress), sizeof(SOCKADDR_IN)))
	{
		CRASH();
		return false;
	}

	if (::listen(m_ListenerSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		CRASH();
		return false;
	}

	// CP�� ������ ���
	GIOCP->RegisterToCP(shared_from_this());

	for (int32 i = 0; i < 5; i++)
	{
		IOCP_Overlapped* AcceptOverlapped = new IOCP_Overlapped();
		AcceptOverlapped->SetOwnerObject(shared_from_this()); // RefCount++
		m_arAcceptOverlapped.push_back(AcceptOverlapped);
		// CP�� �ϰ� ��� (���� �غ�)
		RegisterIOCP_Accept(AcceptOverlapped);
	}


	return true;
}

wstring IOCP_Listener::GetIP_Address()
{
	WCHAR buffer[100];
	::InetNtopW(AF_INET, &m_SocketAddress.sin_addr, buffer,
		sizeof(buffer) / sizeof(buffer[0]));
	return wstring(buffer);
}

IN_ADDR IOCP_Listener::IP_to_Addr(const WCHAR* IP)
{
	IN_ADDR addr;
	::InetPtonW(AF_INET, IP, &addr);
	return addr;
}

void IOCP_Listener::RegisterIOCP_Accept(IOCP_Overlapped* acceptStr)
{
	
	shared_ptr<IOCP_Session> NewSession = GSessionMGR->CreateSession();
	
	acceptStr->SetManagedObject(NewSession);
	acceptStr->init();
	acceptStr->SetOverlapType(OverlapType::Accept);

	SOCKET soc = NewSession->GetSocket();

	DWORD DataLen{};
	if (false == SocketMGR::WSA_AcceptEx(m_ListenerSocket, NewSession->GetSocket(),
		NewSession->GetRecvBuffer()->GetCurWritePos(), 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,
		&DataLen, static_cast<LPOVERLAPPED>(acceptStr)))
	{
		const int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			RegisterIOCP_Accept(acceptStr);
		}

	}

}

void IOCP_Listener::Process_IOCP_Accept(IOCP_Overlapped* acceptStr)
{
	// Session�� RefCount++
	shared_ptr<IOCP_Session> SessionRef = acceptStr->GetManagedSession();

	// ����� ���������� ������ ���ϰ� �����ϰ� ����...
	if (false == SocketMGR::SetSocketOption(SessionRef->GetSocket(), OptionName::X_COPY_SOCKET_OPTION, m_ListenerSocket))
	{
		RegisterIOCP_Accept(acceptStr);
		return;
	}

	// ����Ƿ��� Ŭ���� ��Ʈ��ũ ������ �ܾ�´�. 
	SOCKADDR_IN sockAddress;
	int32 sizeOfSockAddr = sizeof(sockAddress);
	// ������ ����� �Ǿ��� �ּҸ� �˻�
	if (::getpeername(SessionRef->GetSocket(), reinterpret_cast<SOCKADDR*>(&sockAddress), &sizeOfSockAddr)
		== SOCKET_ERROR)
	{
		RegisterIOCP_Accept(acceptStr);
		return;
	}
	SessionRef->SetNetworkAddr(sockAddress);
	SessionRef->SetServiceType(ServiceType::SERVER);
	SessionRef->init();

	cout << "Accept OK" << "\n";
	RegisterIOCP_Accept(acceptStr);
}


// ======================= IOCP_Session =======================
IOCP_Session::IOCP_Session() :
	m_IsConnected(false), m_recvBuffer(0x10000) 
{
	// 0x10000 = 64KB
	m_SessionSocket = SocketMGR::CreateSocket();
}

IOCP_Session::~IOCP_Session()
{
	cout << "~IOCP_Session()" << "\n";
	SocketMGR::CloseSocket(m_SessionSocket);
}

void IOCP_Session::init()
{
	// TODO. ���� �ʱ� �۾� �ʿ��Ѱ� ������ �� ���ֱ�.

	// �������� ���� �� �� ������ On
	// RegisterConnect();

	ProcessConnect(); // ���� ó��
}

void IOCP_Session::Send(shared_ptr<SendBuffer> sendBuffer)
{
	// ������ ���ؿ��� ������ Ŭ��� Send�� �� ȣ���.
	// ��, ���������� sendBuffer ä���� Send �� ������� ��.
	
	if (m_IsConnected == false)
		return;

	bool IsRegisterOK = false;
	{
		// �� �ɰ� ť�� ���� ������ �־��ֱ�
		lock_guard<mutex> guard(m);
		m_qSendBuffer.push(sendBuffer);

		// m_IsSendRegisterd�� ���� ���� false�� true �ٲپ��ֱ�
		// RegisterSend�� �� �� �ֵ��� IsRegisterOK�� true�� ����
		if (m_IsSendRegisterd.exchange(true) == false)
			IsRegisterOK = true;
	}
	if (IsRegisterOK)
		RegisterSend();
}


HANDLE IOCP_Session::GetHandle()
{
	return reinterpret_cast<HANDLE>(m_SessionSocket);
}

void IOCP_Session::CP_Work(IOCP_Overlapped* pOverlapped, int32 DataSize)
{
	OverlapType eType = pOverlapped->GetOverlapType();
	switch (eType)
	{
	case OverlapType::Connect:
		ProcessConnect();
		break;
	case OverlapType::DisConnect:
		ProcessDisConnect();
		break;
	case OverlapType::Recv:
		ProcessRecv(DataSize);
		break;
	case OverlapType::Send:
		ProcessSend(DataSize);
		break;
	default:
		break;
	}
	return;
}

void IOCP_Session::SetNetworkAddr(SOCKADDR_IN sockAddr)
{
	m_SockAddr = sockAddr;
}

void IOCP_Session::DisConnect()
{
	if (m_IsConnected.exchange(false) == false)
		return;

	RegisterDisConnect();
}

bool IOCP_Session::RegisterConnect()
{
	if (m_IsConnected == true)
		return false;

	if (m_eServiceType != ServiceType::CLIENT)
		return false;

	if (SocketMGR::SetSocketOption(m_SessionSocket, OptionName::X_REUSEADDR, true)
		== false)
		return false;

	if (SocketMGR::BindAnyAddress(m_SessionSocket, 0) == false)
		return false;

	m_ConnectOverlapped.init();
	m_ConnectOverlapped.SetOwnerObject(shared_from_this());
	m_ConnectOverlapped.SetOverlapType(OverlapType::Connect);

	DWORD numOfBytes = 0;
	if (false == SocketMGR::WSA_ConnectEx(m_SessionSocket, reinterpret_cast<SOCKADDR*>(&m_SockAddr),
		sizeof(m_SockAddr), nullptr, 0, &numOfBytes, &m_ConnectOverlapped))
	{
		int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			m_ConnectOverlapped.SetOwnerObject(nullptr); // RefCount--
			return false;
		}
	}

	return true;
}

void IOCP_Session::ProcessConnect()
{
	{
		// ���� ���̵� �ο�
		lock_guard<mutex> guard(m);
		// GSessionMGR�� �ڱ� �ڽ� ���.
		GSessionMGR->AddSession(static_pointer_cast<IOCP_Session>(shared_from_this()));
	}

	m_ConnectOverlapped.SetOwnerObject(nullptr);
	m_IsConnected.store(true);
	
	// Session�� ��üȭ ��Ű�� ���� �� ��� ���״ٸ�... ���������� �߰� ���� 
	OnConnect(); 

	// ����Ǿ����� Recv ������ ���
	RegisterRecv();
}

bool IOCP_Session::RegisterDisConnect()
{
	m_DisConnectOverlapped.init();
	m_DisConnectOverlapped.SetOwnerObject(shared_from_this());
	m_DisConnectOverlapped.SetOverlapType(OverlapType::DisConnect);

	if (SocketMGR::WSA_DisConnectEx(m_SessionSocket, &m_DisConnectOverlapped,
		TF_REUSE_SOCKET, 0) == false)
	{
		const int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			m_DisConnectOverlapped.SetOwnerObject(nullptr);
			return false;
		}
	}

	return true;
}

void IOCP_Session::ProcessDisConnect()
{
	{
		lock_guard<mutex> guard(m);

		// GSessionMGR�� �ڱ� �ڽ� ����. GIOCP������ �ڱ� ���� & ���� ���̵� �ݳ�
		GSessionMGR->ReleaseSession(m_OwnSessionID);
		GIOCP->ReleaseFromCP(m_OwnObjectID);
	}

	m_IsConnected.store(false);

	// Session�� ��üȭ ��Ű�� ���� �� ��� ���״ٸ�... ���������� �߰� ���� 
	OnDisconnect();

	m_recvBuffer.Reset();
	// ��.. �������� �� Session�� ���� �� �ʿ�����ϱ� ��� ���ص��ǳ�? 
	RegisterDisConnect();
}

bool IOCP_Session::RegisterRecv()
{
	if (m_IsConnected == false)
		return false;

	m_RecvOverlapped.init();
	m_RecvOverlapped.SetOwnerObject(shared_from_this());
	m_RecvOverlapped.SetOverlapType(OverlapType::Recv);

	WSABUF wsaBuf;
	wsaBuf.buf = reinterpret_cast<char*>(m_recvBuffer.GetCurWritePos());
	wsaBuf.len = m_recvBuffer.GetRemainedSize();

	DWORD numOfBytes = 0;
	DWORD flag = 0;
	if (::WSARecv(m_SessionSocket, &wsaBuf, 1, &numOfBytes, &flag, &m_RecvOverlapped, nullptr)
		== SOCKET_ERROR)
	{
		const int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			cout << "Recv Error : " << errorCode << "\n";
			m_RecvOverlapped.SetOwnerObject(nullptr);
			DisConnect();
		}
	}

	return true;
}

void IOCP_Session::ProcessRecv(int32 DataSize)
{
	m_RecvOverlapped.SetOwnerObject(nullptr);
	if (DataSize <= 0)
	{
		DisConnect();
		return;
	}

	if (m_recvBuffer.WriteComplete(DataSize) == false)
	{
		DisConnect();
		return;
	}

	int32 dataSize = m_recvBuffer.GetCurDataSize();

	// ó���� �� ������ ó��
	if (OnRecv(m_recvBuffer.GetCurReadPos(), dataSize) == false)
	{
		DisConnect();
		return;
	}

	if (m_recvBuffer.ReadComplete(dataSize) == false)
	{
		DisConnect();
		return;
	}
	
	// ���� �ʱ�ȭ
	m_recvBuffer.Reset();

	// �ٽ� CP�� �ɷ� ����
	RegisterRecv();
}

void IOCP_Session::RegisterSend()
{
	// ť�� ���� ������ �����޶�� CP�� ���
	if (m_IsConnected == false)
		return;

	m_SendOverlapped.init();
	m_SendOverlapped.SetOwnerObject(shared_from_this());
	m_SendOverlapped.SetOverlapType(OverlapType::Send);
	
	{
		// ���� �����ͱ��� ©�� m_SendOverlapped ���ο� ����
		lock_guard<mutex> guard(m);
		int32 TotalWriteSize{};
		while (!m_qSendBuffer.empty())
		{
			// �׿��ִ� �����Ϳ��� �ϳ� ��������
			shared_ptr<SendBuffer> TmpSendBuffer = m_qSendBuffer.front();
			TotalWriteSize += TmpSendBuffer->GetWriteSize(); 

			// �� ���� ������ ����� 128KB �̻��̶�� �ϴ� ¥����
			if (TotalWriteSize > 0x20000)
				break;

			m_qSendBuffer.pop();
			m_SendOverlapped.m_vSendBuffer.push_back(TmpSendBuffer);
		}
	}

	// Scatter-Gather ��� ����
	vector<WSABUF> wsaBufs{};
	wsaBufs.reserve(m_SendOverlapped.m_vSendBuffer.size()); 
	for (shared_ptr<SendBuffer> sendBuffer : m_SendOverlapped.m_vSendBuffer)
	{
		WSABUF wsaBuf;
		wsaBuf.buf = reinterpret_cast<char*>(sendBuffer->GetBuffer());
		wsaBuf.len = static_cast<ULONG>(sendBuffer->GetWriteSize());

		wsaBufs.push_back(wsaBuf);
	}

	DWORD sentBytes = 0;
	DWORD wsaBuffsSize = static_cast<DWORD>(wsaBufs.size());

	// n���� SendBuffer �ѹ��� ������.
	if (::WSASend(m_SessionSocket, wsaBufs.data(), wsaBuffsSize,
		&sentBytes, 0, &m_SendOverlapped, nullptr)
		== SOCKET_ERROR)
	{
		int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			m_SendOverlapped.SetOwnerObject(nullptr);
			m_SendOverlapped.m_vSendBuffer.clear();
			m_IsSendRegisterd.store(false);
			DisConnect();
			return;
		}
	}
}

void IOCP_Session::ProcessSend(int32 DataSize)
{
	m_SendOverlapped.SetOwnerObject(nullptr);
	m_SendOverlapped.m_vSendBuffer.clear();

	if (DataSize <= 0)
	{
		DisConnect();
		return;
	}


	OnSend(DataSize);

	if (m_qSendBuffer.empty())
		m_IsSendRegisterd.store(false);
	else
		RegisterSend();
}