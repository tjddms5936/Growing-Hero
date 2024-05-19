#pragma once
#include <sql.h>
#include <sqlext.h>

class DB
{
public:
	bool Connect(SQLHENV henv, const WCHAR* connectionString);
	void Clear();

	bool Query(const WCHAR* query);
	bool Fetch(); // 데이터 결과 있으면 받아오는 함수
	int32 GetRowCount();
	void Unbind();

public:
	// SQL 쿼리를 실행할 때 인자들을 넘겨줄 때 사용
	// paramIndex : 넘겨 줄 인자가 여러개 될 수도 있으니까 몇번째 인자인지 의미
	// cType : 타입 맞추기 위함.
	// 실행할 때 넘겨주는 인자들
	bool BindParam(SQLUSMALLINT paramIndex, SQLUSMALLINT cType, SQLUSMALLINT sqlType, SQLLEN len, SQLPOINTER ptr, SQLLEN* index);
	bool BindCol(SQLUSMALLINT columIndex, SQLUSMALLINT cType, SQLLEN len, SQLPOINTER value, SQLLEN* index); // 실행한 다음 데이터를 긁어올 때
	void HandleError(SQLRETURN ret);

private:
	bool check(SQLRETURN ret);

private:
	// 데이터베이스 커넥션.
	SQLHDBC m_Connection = SQL_NULL_HANDLE;
	// 상태
	SQLHSTMT m_Statement = SQL_NULL_HANDLE;

};

