// RLink.h: interface for the RLink class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RLINK_H__EC6EF410_3C4A_11D2_A715_204C4F4F5020__INCLUDED_)
#define AFX_RLINK_H__EC6EF410_3C4A_11D2_A715_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class Record;
class RKey;
#include "List.h"
#include "QRestr.h"
class SqlStmt;
class DbCursor;
class DataBase;

class RLink : public BaseList
{
	Record* psrcrec;
	Record* ptrgrec;
	RKey*	psrckey;
	RKey*	ptrgkey;

public:
//Construction
	RLink(Record* pr, RKey* prk, RLink*& pHead) :
	  BaseList((BaseList*&)pHead), psrcrec(pr), psrckey(prk), 
	  ptrgrec(NULL), ptrgkey(NULL){};
	void SetTarget(Record* pr, RKey* prk){ptrgrec = pr; ptrgkey = prk;};//For use by class Record ONLY
	QRestr* CreateQRestr(Query* pQuery, QRestr* pHead);
	~RLink(){};

	void Select(SqlStmt& str, bool bWhere);
	void Bind(DbCursor* pCur);
	void Bind(DataBase* pDB);
//Operations
	bool	ReturnToSrc();
	bool SetDefaults(Record* pr);
//	QRestr*	GetSrcQRestr();
};


#endif // !defined(AFX_RLINK_H__EC6EF410_3C4A_11D2_A715_204C4F4F5020__INCLUDED_)
