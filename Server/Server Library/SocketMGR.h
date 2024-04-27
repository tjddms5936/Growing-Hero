#pragma once

enum OptionName
{
	X_LINGER = SO_LINGER,
	X_REUSEADDR = SO_REUSEADDR,
	X_RCVBUF = SO_RCVBUF,
	X_SNDBUF = SO_SNDBUF,
	X_TCP_NO_DELAY = TCP_NODELAY,
	X_COPY_SOCKET_OPTION = SO_UPDATE_ACCEPT_CONTEXT
};

class SocketMGR
{
public:
	/*SocketMGR() = default;
	~SocketMGR() = default;
	SocketMGR(const SocketMGR&) = delete;
	SocketMGR& operator=(const SocketMGR&) = delete;*/

public:
	// ----------- ing -----------
	// 런타임 시 긁어와야 함.
	static LPFN_CONNECTEX WSA_ConnectEx;
	static LPFN_DISCONNECTEX WSA_DisConnectEx;
	static LPFN_ACCEPTEX WSA_AcceptEx;

public:
	static void init();
	static SOCKET CreateSocket();
	static void CloseSocket(SOCKET& socket);
	static bool BindRuntimeFunc(SOCKET socket, GUID guid, LPVOID* func);

	static bool BindAnyAddress(SOCKET socket, uint16 port);

	template<typename T>
	static bool SetSocketOption(SOCKET socket, OptionName optionName, T optionValue);


};

template<typename T>
inline bool SocketMGR::SetSocketOption(SOCKET socket, OptionName optionName, T optionValue)
{
	if (::setsockopt(socket, SOL_SOCKET, optionName, reinterpret_cast<char*>(&optionValue), sizeof(optionValue))
		== SOCKET_ERROR)
	{
		return false;
	}

	return true;
}
