#pragma once
#include "functional"

extern thread_local uint8 TLS_ThreadID;

// 쓰레드 만들어주고 관리
class ThreadMGR
{
public:
	ThreadMGR();
	~ThreadMGR();

public:
	void CreateThread(function<void(void)> CallBackFunc);
	void AllThreadJoin();

private:
	void AddThread();
	void ReleaseThread();

private:
	uint8 m_MaxCpuCnt;
	queue<uint8> m_qCurThreadCnt;

	mutex m;
	vector<thread> m_threads;
};


