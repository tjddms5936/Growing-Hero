#include "pch.h"
#include "ThreadMGR.h"

thread_local uint8 TLS_ThreadID = 0;

ThreadMGR::ThreadMGR() :
	m_MaxCpuCnt{}, m_qCurThreadCnt{}
{
	m_MaxCpuCnt = std::thread::hardware_concurrency();
	for (int i = 1; i <= m_MaxCpuCnt; i++)
	{
		m_qCurThreadCnt.push(i);
	}
}

ThreadMGR::~ThreadMGR()
{
	AllThreadJoin();
}

void ThreadMGR::AddThread()
{
	// 고유 Thread ID 부여
	TLS_ThreadID = m_qCurThreadCnt.front();
	m_qCurThreadCnt.pop();
}

void ThreadMGR::ReleaseThread()
{
	// 고유 Thread ID 반납
	m_qCurThreadCnt.push(TLS_ThreadID);
}

void ThreadMGR::CreateThread(function<void(void)> CallBackFunc)
{
	lock_guard<mutex> guard(m);
	if (m_qCurThreadCnt.empty())
		return;
	m_threads.push_back(std::thread([=]()
		{
			AddThread(); // thread가 만들어지면 InitTLS 해주고
			CallBackFunc(); // callback 함수 실행해주고
			ReleaseThread(); // TLS 정리
		})
	);

	return;
}

void ThreadMGR::AllThreadJoin()
{
	for (thread& t : m_threads)
	{
		if (t.joinable())
			t.join();
	}

	m_threads.clear();
}
