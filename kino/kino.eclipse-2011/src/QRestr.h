// QRestr.h: interface for the QRestr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QRESTR_H__10DF6D42_F923_11D1_A657_204C4F4F5020__INCLUDED_)
#define AFX_QRESTR_H__10DF6D42_F923_11D1_A657_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class Query;
class DbCursor;

class QRestr  
{
	QField* pQField;
	char* data;
	RField* pRField;
	QRestr* next;
//friend class RecordSet;
public:
	QRestr(QField* pqf, const char* str, RField* prf, QRestr* pqr);
//	QRestr(Query* pq, const char* str, QRestr* pqr, Record* pr);
	virtual ~QRestr();
	bool Where(SqlStmt& str, bool bWhere);
	void Bind(DbCursor* pCur);
/*	void GetQFields(QFields& qf, Strs& st);
	const char* GetData(QFields* pqf);
*/
	bool SetDefaults(Record* pr);
public:
	static QRestr* Create(Query* pq, RKey* prk, QRestr* pqr, Record* pr = NULL);
	DECLARE_DICT_MEMORY
};

#endif // !defined(AFX_QRESTR_H__10DF6D42_F923_11D1_A657_204C4F4F5020__INCLUDED_)
