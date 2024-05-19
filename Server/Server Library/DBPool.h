#pragma once
#include "DB.h"

class DBPool
{
public:
	DBPool();
	~DBPool();

	// WCHAR : 어떤 DB랑 어떤 환경으로 어떤 조건으로 연결할지 등등 받아주는 부분
	bool CreateDB(int32 DB_Count, const WCHAR* ConnectionString);
	void Clear();

	DB* Pop();
	void Push(DB* DBConnection);

private:
	mutex m;
	SQLHENV m_environment = SQL_NULL_HANDLE;
	vector<DB*> m_arDB;
};

