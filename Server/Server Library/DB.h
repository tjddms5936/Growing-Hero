#pragma once
#include <sql.h>
#include <sqlext.h>

class DB
{
public:
	bool Connect(SQLHENV henv, const WCHAR* connectionString);
	void Clear();

	bool Query(const WCHAR* query);
	bool Fetch(); // ������ ��� ������ �޾ƿ��� �Լ�
	int32 GetRowCount();
	void Unbind();

public:
	// SQL ������ ������ �� ���ڵ��� �Ѱ��� �� ���
	// paramIndex : �Ѱ� �� ���ڰ� ������ �� ���� �����ϱ� ���° �������� �ǹ�
	// cType : Ÿ�� ���߱� ����.
	// ������ �� �Ѱ��ִ� ���ڵ�
	bool BindParam(SQLUSMALLINT paramIndex, SQLUSMALLINT cType, SQLUSMALLINT sqlType, SQLLEN len, SQLPOINTER ptr, SQLLEN* index);
	bool BindCol(SQLUSMALLINT columIndex, SQLUSMALLINT cType, SQLLEN len, SQLPOINTER value, SQLLEN* index); // ������ ���� �����͸� �ܾ�� ��
	void HandleError(SQLRETURN ret);

private:
	bool check(SQLRETURN ret);

private:
	// �����ͺ��̽� Ŀ�ؼ�.
	SQLHDBC m_Connection = SQL_NULL_HANDLE;
	// ����
	SQLHSTMT m_Statement = SQL_NULL_HANDLE;

};

