#include "pch.h"
#include "DB.h"

bool DB::Connect(SQLHENV henv, const WCHAR* connectionString)
{
	// m_Connection �ڵ� �����
	if (::SQLAllocHandle(SQL_HANDLE_DBC, henv, &m_Connection) != SQL_SUCCESS)
		return false;

	// connectionString�� �̿��ؼ� ���������� DB ���� ���ֱ�
	WCHAR stringBuffer[MAX_PATH] = { 0 };
	::wcscpy_s(stringBuffer, connectionString);

	// �Լ��� ȣ���� ������ ����� �޾ƿ� ����
	WCHAR resultBuffer[MAX_PATH] = { 0 };
	SQLSMALLINT resultStringLen{};

	// WCHAR ����ϰ� �����ϱ� W���� ���
	SQLRETURN ret = ::SQLDriverConnectW(
		m_Connection,
		NULL,
		reinterpret_cast<SQLWCHAR*>(stringBuffer),
		_countof(stringBuffer),
		reinterpret_cast<SQLWCHAR*>(resultBuffer),
		_countof(resultBuffer),
		&resultStringLen,
		SQL_DRIVER_NOPROMPT
	);

	// m_Statement �ڵ� �����
	if (::SQLAllocHandle(SQL_HANDLE_STMT, m_Connection, &m_Statement) != SQL_SUCCESS)
		return false;

	return check(ret);
}

void DB::Clear()
{
	if (m_Connection != SQL_NULL_HANDLE)
	{
		::SQLFreeHandle(SQL_HANDLE_DBC, m_Connection);
		m_Connection = SQL_NULL_HANDLE;
	}
	if (m_Statement != SQL_NULL_HANDLE)
	{
		::SQLFreeHandle(SQL_HANDLE_STMT, m_Statement);
		m_Statement = SQL_NULL_HANDLE;
	}
}

bool DB::Query(const WCHAR* query)
{
	// SQL ������ �޾Ƽ� �����ش޶�� �ϴ� ����
	SQLRETURN ret =::SQLExecDirectW(m_Statement, (SQLWCHAR*)query, SQL_NTSL);
	return check(ret);
}

bool DB::Fetch()
{
	// �����͸� �ܾ�� �� ���
	SQLRETURN ret = ::SQLFetch(m_Statement);
	return check(ret);
}

int32 DB::GetRowCount()
{
	SQLLEN count{};
	SQLRETURN ret = ::SQLRowCount(m_Statement, &count);
	
	if (check(ret) == true)
		return static_cast<int32>(count);

	return -1;
}

void DB::Unbind()
{
	// BindParam, BindCol �� ������ �ϱ� ���� ���ε� ���� ������� ��.
	::SQLFreeStmt(m_Statement, SQL_UNBIND);
	::SQLFreeStmt(m_Statement, SQL_RESET_PARAMS);
	::SQLFreeStmt(m_Statement, SQL_CLOSE);

}

bool DB::BindParam(SQLUSMALLINT paramIndex, SQLUSMALLINT cType, SQLUSMALLINT sqlType, SQLLEN len, SQLPOINTER ptr, SQLLEN* index)
{
	SQLRETURN ret = ::SQLBindParameter(m_Statement, paramIndex, SQL_PARAM_INPUT, cType, sqlType, len, 0, ptr, 0, index);
	return check(ret);
}

bool DB::BindCol(SQLUSMALLINT columIndex, SQLUSMALLINT cType, SQLLEN len, SQLPOINTER value, SQLLEN* index)
{
	SQLRETURN ret = ::SQLBindCol(m_Statement, columIndex, cType, value, len, index);
	return check(ret);
}

void DB::HandleError(SQLRETURN ret)
{
	if (ret == SQL_SUCCESS)
		return;
	SQLSMALLINT index = 1;
	SQLWCHAR sqlState[MAX_PATH] = { 0 };
	SQLINTEGER nativeErr{};
	SQLWCHAR errMsg[MAX_PATH] = { 0 };
	SQLSMALLINT msgLen{};
	SQLRETURN Errret{};

	while (true)
	{
		// ��Ȯ�� ���� �޼��� ���� �Լ�
		Errret = ::SQLGetDiagRecW(
			SQL_HANDLE_STMT,
			m_Statement,
			index,
			sqlState,
			&nativeErr,
			errMsg,
			_countof(errMsg),
			&msgLen
		);

		if (Errret == SQL_NO_DATA || Errret == SQL_SUCCESS || Errret == SQL_SUCCESS_WITH_INFO)
			break;

		// �ѱ���ε� ����ϰ� ����
		wcout.imbue(locale("kor"));
		wcout << errMsg << endl;

		index++;
	}

}

bool DB::check(SQLRETURN ret)
{
	if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
		return true;
	
	if (ret == SQL_NO_DATA)
		return false;

	HandleError(ret);
	return false;
}
