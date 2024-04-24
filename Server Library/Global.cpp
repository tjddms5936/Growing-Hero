#include "pch.h"
#include "Global.h"
#include "SessionMGR.h"
#include "SocketMGR.h"
#include "IOCP.h"
#include "ThreadMGR.h"

class SessionMGR* GSessionMGR = nullptr;
class IOCP* GIOCP = nullptr;
class ThreadMGR* GThreadMGR = nullptr;

class GlobalVal
{
	enum
	{
		MAX_IOCP_OBJECT_CNT = 100
		// MAX_IOCP_OBJECT_CNT = 65534
	};
public:
	GlobalVal()
	{
		GSessionMGR = new SessionMGR(MAX_IOCP_OBJECT_CNT);
		SocketMGR::init();

		GIOCP = new IOCP(MAX_IOCP_OBJECT_CNT);

		GThreadMGR = new ThreadMGR();
	}
	~GlobalVal()
	{
		delete GSessionMGR;
		delete GIOCP;
		delete GThreadMGR;
	}
	GlobalVal(const GlobalVal&) = delete;
	GlobalVal& operator=(const GlobalVal&) = delete;
}GGlobalVal;
