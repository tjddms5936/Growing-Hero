#include "pch.h"
#include "IOCP_Object.h"
#include "IOCP.h"
#include "ThreadMGR.h"
#include "SessionMGR.h"
#include "GameSession.h"
#include "ClientPacketHandler.h"
#include "ChildSessionMGR.h"

int main()
{
	// 윈속 초기화는 SocketMGR에서 런타임시에 시작된다.
	// WSAAcceptEX와 같은 비동기 함수를 긁어오기 위해 선행되어야 하기 때문
	ClientPacketHandler::init();
	
	shared_ptr<IOCP_Listener> listener = make_shared<IOCP_Listener>();
	listener->init(make_shared<GameSession>);
	// listener->StartListen(L"127.0.0.1", 7777); // 루프백 어드레스
	listener->StartListen(L"172.31.5.81", 7777); // AWS EC2사설IP (포트포워딩필요)

	for (int i = 0; i < 5; i++)
	{
		GThreadMGR->CreateThread([=]()
			{
				while (true)
				{
					GIOCP->WorkSearch();
				}
			});
	}

	GThreadMGR->AllThreadJoin();
	::WSACleanup();
}

