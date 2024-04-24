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
	// ���� Thread ID �ο�
	TLS_ThreadID = m_qCurThreadCnt.front();
	m_qCurThreadCnt.pop();
}

void ThreadMGR::ReleaseThread()
{
	// ���� Thread ID �ݳ�
	m_qCurThreadCnt.push(TLS_ThreadID);
}

void ThreadMGR::CreateThread(function<void(void)> CallBackFunc)
{
	lock_guard<mutex> guard(m);
	if (m_qCurThreadCnt.empty())
		return;
	m_threads.push_back(std::thread([=]()
		{
			AddThread(); // thread�� ��������� InitTLS ���ְ�
			CallBackFunc(); // callback �Լ� �������ְ�
			ReleaseThread(); // TLS ����
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
