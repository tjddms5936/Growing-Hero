#include "pch.h"
#include "Global.h"
#include "SessionMGR.h"
#include "SocketMGR.h"
#include "IOCP.h"
#include "ThreadMGR.h"
#include "DBPool.h"

class SessionMGR*	GSessionMGR = nullptr;
class IOCP*			GIOCP = nullptr;
class ThreadMGR*	GThreadMGR = nullptr;
class DBPool*		GDB_Pool = nullptr;

class GlobalVal
{
	enum
	{
		MAX_IOCP_OBJECT_CNT = 10000
		// MAX_IOCP_OBJECT_CNT = 65534
	};
public:
	GlobalVal()
	{
		GSessionMGR = new SessionMGR(MAX_IOCP_OBJECT_CNT);
		SocketMGR::init();

		GIOCP = new IOCP(MAX_IOCP_OBJECT_CNT);
		GThreadMGR = new ThreadMGR();
		GDB_Pool = new DBPool();
	}
	~GlobalVal()
	{
		delete GSessionMGR;
		delete GIOCP;
		delete GThreadMGR;
		delete GDB_Pool;
	}
	GlobalVal(const GlobalVal&) = delete;
	GlobalVal& operator=(const GlobalVal&) = delete;
}GGlobalVal;
