#include "pch.h"
#include "IOCP_Object.h"
#include "IOCP.h"
#include "ThreadMGR.h"
#include "SessionMGR.h"
#include "GameSession.h"
#include "ClientPacketHandler.h"
#include "ChildSessionMGR.h"
#include "DBPool.h"
#include "DB.h"

int main()
{
	ASSERT_CRASH(GDB_Pool->CreateDB(1, L"Driver={ODBC Driver 17 for SQL Server};Server=(localdb)\\ProjectModels;Database=ServerDB;Trusted_Connection=yes;"));
	
	// 테이블 만들기
	{
		auto query = L"									\
			DROP TABLE IF EXISTS [dbo].[NickName]		\
			CREATE TABLE [dbo].[NickName]				\
			(											\
				[id] INT NOT NULL PRIMARY KEY IDENTITY,	\
				[NickName] NVARCHAR(255) NULL			\
			);";
		
		DB* DBConnection = GDB_Pool->Pop();
		ASSERT_CRASH(DBConnection->Query(query));
		GDB_Pool->Push(DBConnection);
	}

	// 데이터 추가
	for (int32 i = 0; i < 3; i++)
	{
		DB* DBConnection = GDB_Pool->Pop();
		// 기존에 바인딩 된 정보 날림
		DBConnection->Unbind();

		// 넘길 인자 바인딩
		SQLWCHAR NickName[] = L"ExampleNickname"; // 유니코드 문자열
		SQLLEN len{};
		ASSERT_CRASH(DBConnection->BindParam(1, SQL_C_WCHAR, SQL_WVARCHAR, wcslen(NickName), NickName, &len));

		// SQL 실행
		// [dbo].[NickName]이라는 테이블에 NickName 항목에다가 ?값을 넣어줘라. 의미  
		ASSERT_CRASH(DBConnection->Query(L"INSERT INTO [dbo].[NickName]([NickName]) VALUES(?)"));
		GDB_Pool->Push(DBConnection);
	}

	// 데이터 읽기
	{
		DB* DBConnection = GDB_Pool->Pop();
		DBConnection->Unbind();

		// 넘길 인자 바인딩
		SQLWCHAR NickName[] = L"ExampleNickname"; // 유니코드 문자열
		// wstring NickName = L"ExampleNickname";
		SQLLEN len{};
		ASSERT_CRASH(DBConnection->BindParam(1, SQL_C_CHAR, SQL_CHAR, wcslen(NickName), NickName, &len));

		// Select문은 받아서 꺼내와야 함.
		// 결과물을 어디다 저장할지도 지정해줘야 함.
		int32 OutId{};
		SQLLEN OutIdLen{};
		ASSERT_CRASH(DBConnection->BindCol(1, SQL_C_LONG, sizeof(OutId), &OutId, &OutIdLen));

		SQLWCHAR OutNickName[] = L"";
		SQLLEN OutNamelen{};
		ASSERT_CRASH(DBConnection->BindCol(2, SQL_C_CHAR, wcslen(NickName), OutNickName, &OutNamelen));

		ASSERT_CRASH(DBConnection->Query(L"SELECT id, NickName FROM [dbo].[NickName] WHERE NickName = (?)"));

		// 테스트의 경우 3개의 데이터를 넣었으니 3개의 행을 뱉어줄 것이다. 
		while (DBConnection->Fetch())
		{
			cout << "id : " << OutId << "\t" << "OutNickName : ";
			wcout << NickName << std::endl;
		}
 		GDB_Pool->Push(DBConnection);
	}


	// 윈속 초기화는 SocketMGR에서 런타임시에 시작된다.
	// WSAAcceptEX와 같은 비동기 함수를 긁어오기 위해 선행되어야 하기 때문
	ClientPacketHandler::init();
	
	shared_ptr<IOCP_Listener> listener = make_shared<IOCP_Listener>();
	listener->init(make_shared<GameSession>);
	listener->StartListen(L"127.0.0.1", 7777); // 루프백 어드레스
	// listener->StartListen(L"172.31.5.81", 7777); // AWS EC2사설IP (포트포워딩필요)

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

