#pragma once
#include "IOCP_Overlapped.h"
#include "RecvBuffer.h"
#include "SendBuffer.h"
#include <functional>

using SessionMaker = std::function<shared_ptr<IOCP_Session>(void)>;

// ======================= IOCP_Object =======================
class IOCP_Object : public enable_shared_from_this<IOCP_Object>
{
protected:
	enum ServiceType
	{
		CLIENT,
		SERVER
	};

public:
	virtual HANDLE GetHandle() abstract;
	virtual void CP_Work(IOCP_Overlapped* pOverlapped, int32 DataSize= 0) abstract;

	void SetOwnObjectID(uint32 ID) { m_OwnObjectID = ID; }
	uint32 GetOwnObjectID() { return m_OwnObjectID; };

protected:
	uint32 m_OwnObjectID; // GIOCP->RegisterToCP 타이밍에 부여됨

	std::mutex m;
};

// ======================= IOCP_Listener =======================
class IOCP_Listener : public IOCP_Object
{
public:
	 IOCP_Listener() = default;
	 ~IOCP_Listener();
	
public:
	virtual HANDLE GetHandle() override;
	virtual void CP_Work(IOCP_Overlapped* pOverlapped, int32 DataSize) override;

	void init(SessionMaker factory);
	bool StartListen(wstring IP, uint16 Port);
	wstring GetIP_Address();

private:
	IN_ADDR IP_to_Addr(const WCHAR* IP);
	
private:
	// IOCP 일감 주기 & 일처리
	void RegisterIOCP_Accept(IOCP_Overlapped* acceptStr);
	void Process_IOCP_Accept(IOCP_Overlapped* acceptStr);

private:
	SOCKET m_ListenerSocket;
	// IOCP_Overlapped* m_AcceptOverlapped;
	vector<IOCP_Overlapped*> m_arAcceptOverlapped;
	SOCKADDR_IN m_SocketAddress;
};	

// ======================= IOCP_Session =======================

// Session은 연결된 클라를 1:1로 관리하는 창구 역할이다. 
// 연결된 클라의 네트워크 정보. recv, send, disconnect, connect와 관련된 일을 처리한다.
// connect는. 서버와 서버 연동 제외하고 accept 마무리 되었을 때 처리 할 일. 그리고 다시 CP에 걸기
// disconnect는 데이터가 0이하로 보내진다거나 할 때등. 여러 부분에서 연결이 끊긴 탐지를 할 수 있다. 이때 불리우고 CP에 등록하러 떠남.
// processdisconnect는 cp에서 일감 처리하는건데, 뭐 refCount관리하거나 필요한거 끊어주는 작업 진행.

class IOCP_Session : public IOCP_Object
{
public:
	enum SessionType
	{
		NONE_SESSION = 0,
		GAME_SESSION

	};

public:
	IOCP_Session();
	virtual ~IOCP_Session();


public:
	virtual HANDLE GetHandle() override;
	virtual void CP_Work(IOCP_Overlapped* pOverlapped, int32 DataSize) override;

public:
	void init();
	void Send(shared_ptr<SendBuffer> sendBuffer);
	void DisConnect();
public:
	SOCKET GetSocket() { return m_SessionSocket; }
	void SetNetworkAddr(SOCKADDR_IN sockAddr);
	bool IsConnected() { return m_IsConnected; }
	RecvBuffer* GetRecvBuffer(){ return &m_recvBuffer; }

	uint32 GetSessionID() { return m_OwnSessionID; }
	void SetSessionID(uint16 SessionID) { m_OwnSessionID = SessionID; }

	ServiceType GetServiceType() { return m_eServiceType; }
	void SetServiceType(ServiceType eType) { m_eServiceType = eType; }

	SessionType GetSessionType() { return m_eSessionType; }
	void SetSessionType(SessionType eSessiontType) { m_eSessionType = eSessiontType; }

public:
	// Completion Port에 일감 등록 & CompletedQueue에서 얻어온 일감 처리 
	bool RegisterConnect();
	void ProcessConnect();
	virtual void OnConnect() abstract; // 흠.. Session을 상속받아서 구분 할 일이 있을까?

	bool RegisterDisConnect();
	void ProcessDisConnect();
	virtual void OnDisconnect() abstract;

	bool RegisterRecv();
	void ProcessRecv(int32 DataSize);
	virtual bool OnRecv(unsigned char* recvBuffer, int32 dataSize) abstract;

	void RegisterSend();
	void ProcessSend(int32 DataSize);
	virtual void OnSend(int32 DataSize) abstract;

public:
	IOCP_Overlapped m_ConnectOverlapped;

private:
	SOCKET m_SessionSocket;
	SOCKADDR_IN m_SockAddr;
	std::atomic<bool> m_IsConnected;
	uint16 m_OwnSessionID; // SessionMGR->AddSession 타이밍에 부여됨

	IOCP_Overlapped m_DisConnectOverlapped;
	IOCP_Overlapped m_RecvOverlapped;
	IOCP_Overlapped m_SendOverlapped;

	RecvBuffer m_recvBuffer;

	queue<shared_ptr<SendBuffer>> m_qSendBuffer;
	atomic<bool> m_IsSendRegisterd;

	ServiceType m_eServiceType;
	SessionType m_eSessionType = NONE_SESSION;
};
