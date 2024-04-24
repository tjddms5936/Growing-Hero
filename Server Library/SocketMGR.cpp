#include "pch.h"
#include "SocketMGR.h"

LPFN_CONNECTEX		SocketMGR::WSA_ConnectEx = nullptr;
LPFN_DISCONNECTEX	SocketMGR::WSA_DisConnectEx = nullptr;
LPFN_ACCEPTEX		SocketMGR::WSA_AcceptEx = nullptr;

void SocketMGR::init()
{
	// ----------- ing -----------
	// 런타임시 함수 긁어오기 작업
	// 윈속이 초기화 되어야 긁어올 수 있다.
	WSADATA wsaData{};
	if (0 != ::WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		std::cout << "--WSAStartup Error" << "\n";
		return;
	}

	SOCKET Socket = CreateSocket();
	BindRuntimeFunc(Socket, WSAID_CONNECTEX, reinterpret_cast<LPVOID*>(&WSA_ConnectEx));
	BindRuntimeFunc(Socket, WSAID_DISCONNECTEX, reinterpret_cast<LPVOID*>(&WSA_DisConnectEx));
	BindRuntimeFunc(Socket, WSAID_ACCEPTEX, reinterpret_cast<LPVOID*>(&WSA_AcceptEx));

	CloseSocket(Socket);
}

SOCKET SocketMGR::CreateSocket()
{
	// overlapped 계열의 비동기 함수 사용 가능 설정
	return ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
}

void SocketMGR::CloseSocket(SOCKET& socket)
{
	if (socket == INVALID_SOCKET)
		return;

	::closesocket(socket);
	socket = INVALID_SOCKET;
}

bool SocketMGR::BindRuntimeFunc(SOCKET socket, GUID guid, LPVOID* func)
{
	DWORD bytes = 0;

	// 런타임에 긁어와야 할 함수들의 주소를 얻어오는 함수들임.
	if (::WSAIoctl(socket, SIO_GET_EXTENSION_FUNCTION_POINTER, (LPVOID)&guid, sizeof(guid),
		func, sizeof(*func), &bytes, NULL, NULL) == SOCKET_ERROR)
	{
		cout << "Runtime Fail" << "\n";
		return false;
	}
	cout << "Runtime Success" << "\n";
	return true;
}

bool SocketMGR::BindAnyAddress(SOCKET socket, uint16 port)
{
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = ::htonl(INADDR_ANY);
	addr.sin_port = ::htons(port);
	return SOCKET_ERROR != ::bind(socket, reinterpret_cast<const SOCKADDR*>(&addr), sizeof(addr));
}
