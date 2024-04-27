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
	// IOCP랑 SessionMGR 관련 init 해주기. 그래야 다음 통과 가능
	GIOCP->init();
	GSessionMGR->init(factory);

	// overlapped 계열의 비동기 함수 설정 완료된 채로 반환
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
	
	// 네트워크 설정
	memset(&m_SocketAddress, 0, sizeof(m_SocketAddress));
	m_SocketAddress.sin_family = AF_INET;
	m_SocketAddress.sin_addr = IP_to_Addr(IP.c_str());
	m_SocketAddress.sin_port = htons(Port);

	// 바인드
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

	// CP에 리스너 등록
	GIOCP->RegisterToCP(shared_from_this());

	for (int32 i = 0; i < 5; i++)
	{
		IOCP_Overlapped* AcceptOverlapped = new IOCP_Overlapped();
		AcceptOverlapped->SetOwnerObject(shared_from_this()); // RefCount++
		m_arAcceptOverlapped.push_back(AcceptOverlapped);
		// CP에 일감 등록 (받을 준비)
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
	// Session의 RefCount++
	shared_ptr<IOCP_Session> SessionRef = acceptStr->GetManagedSession();

	// 연결된 소켓정보를 리스너 소켓과 동일하게 적용...
	if (false == SocketMGR::SetSocketOption(SessionRef->GetSocket(), OptionName::X_COPY_SOCKET_OPTION, m_ListenerSocket))
	{
		RegisterIOCP_Accept(acceptStr);
		return;
	}

	// 연결되려는 클라의 네트워크 정보를 긁어온다. 
	SOCKADDR_IN sockAddress;
	int32 sizeOfSockAddr = sizeof(sockAddress);
	// 소켓이 연결된 피어의 주소를 검색
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
	// TODO. 뭔가 초기 작업 필요한거 있으면 다 해주기.

	// 서버끼리 연동 할 일 있으면 On
	// RegisterConnect();

	ProcessConnect(); // 연결 처리
}

void IOCP_Session::Send(shared_ptr<SendBuffer> sendBuffer)
{
	// 컨텐츠 수준에서 서버가 클라로 Send할 때 호출됨.
	// 즉, 컨텐츠에서 sendBuffer 채워서 Send 빵 때려줘야 함.
	
	if (m_IsConnected == false)
		return;

	bool IsRegisterOK = false;
	{
		// 락 걸고 큐에 보낼 데이터 넣어주기
		lock_guard<mutex> guard(m);
		m_qSendBuffer.push(sendBuffer);

		// m_IsSendRegisterd의 현재 값이 false면 true 바꾸어주기
		// RegisterSend에 들어갈 수 있도록 IsRegisterOK도 true로 변경
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
		// 고유 아이디 부여
		lock_guard<mutex> guard(m);
		// GSessionMGR에 자기 자신 등록.
		GSessionMGR->AddSession(static_pointer_cast<IOCP_Session>(shared_from_this()));
	}

	m_ConnectOverlapped.SetOwnerObject(nullptr);
	m_IsConnected.store(true);
	
	// Session을 구체화 시키기 위해 더 상속 시켰다면... 내부적으로 추가 구현 
	OnConnect(); 

	// 연결되었으면 Recv 받으러 출발
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

		// GSessionMGR에 자기 자신 해제. GIOCP에서도 자기 해제 & 고유 아이디 반납
		GSessionMGR->ReleaseSession(m_OwnSessionID);
		GIOCP->ReleaseFromCP(m_OwnObjectID);
	}

	m_IsConnected.store(false);

	// Session을 구체화 시키기 위해 더 상속 시켰다면... 내부적으로 추가 구현 
	OnDisconnect();

	m_recvBuffer.Reset();
	// 음.. 끊겼으면 이 Session도 이제 더 필요없으니까 등록 안해도되나? 
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

	// 처리할 거 있으면 처리
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
	
	// 버퍼 초기화
	m_recvBuffer.Reset();

	// 다시 CP에 걸러 가기
	RegisterRecv();
}

void IOCP_Session::RegisterSend()
{
	// 큐에 쌓인 데이터 보내달라고 CP에 등록
	if (m_IsConnected == false)
		return;

	m_SendOverlapped.init();
	m_SendOverlapped.SetOwnerObject(shared_from_this());
	m_SendOverlapped.SetOverlapType(OverlapType::Send);
	
	{
		// 적정 데이터까지 짤라서 m_SendOverlapped 내부에 저장
		lock_guard<mutex> guard(m);
		int32 TotalWriteSize{};
		while (!m_qSendBuffer.empty())
		{
			// 쌓여있는 데이터에서 하나 꺼내오기
			shared_ptr<SendBuffer> TmpSendBuffer = m_qSendBuffer.front();
			TotalWriteSize += TmpSendBuffer->GetWriteSize(); 

			// 총 보낼 데이터 사이즈가 128KB 이상이라면 일단 짜르기
			if (TotalWriteSize > 0x20000)
				break;

			m_qSendBuffer.pop();
			m_SendOverlapped.m_vSendBuffer.push_back(TmpSendBuffer);
		}
	}

	// Scatter-Gather 기법 적용
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

	// n개의 SendBuffer 한번에 보내기.
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