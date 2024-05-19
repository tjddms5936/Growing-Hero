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
	
	// ȯ�� �����
	if (::SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_environment) != SQL_SUCCESS)
		return false;

	// ������ ODBC ���� ����
	if (::SQLSetEnvAttr(m_environment, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), 0) != SQL_SUCCESS)
		return false;

	// ���ڷ� ���� DB_Count��ŭ ������ �ξ��ֱ�
	// ���� �����忡�� DB���� �ϰԵǸ� �����常ŭ ����DB������ָ��
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

	// ����� DB�鵵 �� ����
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
