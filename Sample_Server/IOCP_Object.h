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
	uint32 m_OwnObjectID; // GIOCP->RegisterToCP Ÿ�ֿ̹� �ο���

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
	// IOCP �ϰ� �ֱ� & ��ó��
	void RegisterIOCP_Accept(IOCP_Overlapped* acceptStr);
	void Process_IOCP_Accept(IOCP_Overlapped* acceptStr);

private:
	SOCKET m_ListenerSocket;
	// IOCP_Overlapped* m_AcceptOverlapped;
	vector<IOCP_Overlapped*> m_arAcceptOverlapped;
	SOCKADDR_IN m_SocketAddress;
};	

// ======================= IOCP_Session =======================

// Session�� ����� Ŭ�� 1:1�� �����ϴ� â�� �����̴�. 
// ����� Ŭ���� ��Ʈ��ũ ����. recv, send, disconnect, connect�� ���õ� ���� ó���Ѵ�.
// connect��. ������ ���� ���� �����ϰ� accept ������ �Ǿ��� �� ó�� �� ��. �׸��� �ٽ� CP�� �ɱ�
// disconnect�� �����Ͱ� 0���Ϸ� �������ٰų� �� ����. ���� �κп��� ������ ���� Ž���� �� �� �ִ�. �̶� �Ҹ���� CP�� ����Ϸ� ����.
// processdisconnect�� cp���� �ϰ� ó���ϴ°ǵ�, �� refCount�����ϰų� �ʿ��Ѱ� �����ִ� �۾� ����.

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
	// Completion Port�� �ϰ� ��� & CompletedQueue���� ���� �ϰ� ó�� 
	bool RegisterConnect();
	void ProcessConnect();
	virtual void OnConnect() abstract; // ��.. Session�� ��ӹ޾Ƽ� ���� �� ���� ������?

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
	uint16 m_OwnSessionID; // SessionMGR->AddSession Ÿ�ֿ̹� �ο���

	IOCP_Overlapped m_DisConnectOverlapped;
	IOCP_Overlapped m_RecvOverlapped;
	IOCP_Overlapped m_SendOverlapped;

	RecvBuffer m_recvBuffer;

	queue<shared_ptr<SendBuffer>> m_qSendBuffer;
	atomic<bool> m_IsSendRegisterd;

	ServiceType m_eServiceType;
	SessionType m_eSessionType = NONE_SESSION;
};
