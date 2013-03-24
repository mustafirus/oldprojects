// Query.h: interface for the Query class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUERY_H__1ACBEBB2_DE6E_11D1_A60C_204C4F4F5020__INCLUDED_)
#define AFX_QUERY_H__1ACBEBB2_DE6E_11D1_A60C_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include "Set.h"

class QField;
class Field;
class QTable;
class Table;
class RKey;

class Query  
{
//	QField* pQField;
	QTable* pQTable;
friend class Record;
friend class RecordSet;
public:
	Query(Table* pt);
	virtual ~Query();
// Creation helpers
	char* GetQFields(const char* str, QFields& qf, QTable* pqt = NULL);
	void GetKeyFields(QTable* pqt, QFields& qtrg);
	operator QTable*(){return pQTable;};
	operator QTable();

//	QField* GetQField(const char* str);
//	void GetQFields(QTable* pqt, QFields& qf, bool child);
	
//	QField* GetQField(Field* pfld, QTable* pqt = NULL);
/*	
	void GetNKeyFields(QTable* pqt, QFields& qtrg);
	QTable* GetLinkTable(Query* pq);

	Query*	Find(Table* pt);
	Query*	Add(Query* pq);
*/
protected:

private:
//	Query*	next;

};

#endif // !defined(AFX_QUERY_H__1ACBEBB2_DE6E_11D1_A60C_204C4F4F5020__INCLUDED_)

