#include "pch.h"
#include "DBPool.h"

DBPool::DBPool()
{

}

DBPool::~DBPool()
{
	Clear();
}

bool DBPool::CreateDB(int32 DB_Count, const WCHAR* ConnectionString)
{
	lock_guard<mutex> guard(m);
	
	// 환경 만들기
	if (::SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_environment) != SQL_SUCCESS)
		return false;

	// 연결할 ODBC 버전 설정
	if (::SQLSetEnvAttr(m_environment, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), 0) != SQL_SUCCESS)
		return false;

	// 인자로 받은 DB_Count만큼 연결을 맺어주기
	// 여러 스레드에서 DB접근 하게되면 스레드만큼 연결DB만들어주면됨
	for (int i = 0; i < DB_Count; i++)
	{
		DB* ConnectDB = new DB();
		if (ConnectDB->Connect(m_environment, ConnectionString) == false)
			return false;
		m_arDB.push_back(ConnectDB);
	}
	return true;
}

void DBPool::Clear()
{
	lock_guard<mutex> guard(m);
	if (m_environment != SQL_NULL_HANDLE)
	{
		::SQLFreeHandle(SQL_HANDLE_ENV, m_environment);
		m_environment = SQL_NULL_HANDLE;
	}

	// 연결된 DB들도 다 꺼줌
	for (DB* ConnectDB : m_arDB)
	{
		ConnectDB->Clear();
		delete ConnectDB;
	}
	m_arDB.clear();
}

DB* DBPool::Pop()
{
	lock_guard<mutex> guard(m);
	if(m_arDB.empty())
		return nullptr;

	DB* connectDB = m_arDB.back();
	m_arDB.pop_back();
	return connectDB;
}

void DBPool::Push(DB* DBConnection)
{
	lock_guard<mutex> guard(m);
	m_arDB.push_back(DBConnection);
}
