#pragma once
#include "DB.h"

class DBPool
{
public:
	DBPool();
	~DBPool();

	// WCHAR : � DB�� � ȯ������ � �������� �������� ��� �޾��ִ� �κ�
	bool CreateDB(int32 DB_Count, const WCHAR* ConnectionString);
	void Clear();

	DB* Pop();
	void Push(DB* DBConnection);

private:
	mutex m;
	SQLHENV m_environment = SQL_NULL_HANDLE;
	vector<DB*> m_arDB;
};

