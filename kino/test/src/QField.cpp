// QField.cpp: implementation of the QField class.
//
//////////////////////////////////////////////////////////////////////

#include "stdx.h"
#include "Dict.h"
#include "SqlStmt.h"
#include "QField.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

QField::QField(QTable* pqt, Field* pf)
{
	pQTable = pqt;
	pField = pf;
	next = NULL;
}

QField::~QField()
{
	ifdel(next);
}

//////////////////////////////////////////////////////////////////////
// Base Operations
//////////////////////////////////////////////////////////////////////

QField* QField::Add(QField* pqf)
{
	if(!pqf)
		return NULL;
	if(next)
		next->Add(pqf);
	else
	{
		next=pqf;
	}
	return pqf;
}

QField* QField::GetQField(Field* pf)
{
	if(pField == pf)
		return this;
	else
	{
		if(next)
			return next->GetQField(pf);
		else
			return Add(new QField(pQTable, pf));
	}
}

void QField::Refresh(SqlStmt& str)
{
	str << pField->name().c_str();
}

void QField::Select(SqlStmt& str)
{
	str << pQTable->AliaS() << "." << pField->name().c_str();
}

void QField::Where(SqlStmt& str)
{
	str << pQTable->AliaS() << "." << pField->name().c_str() << " = " <<(pField->str ? "' ? '" : " ? ");
}

void QField::DeleteWhere(SqlStmt& str)
{
	str << pField->name().c_str() << " = " <<(pField->str ? "' ? '" : " ? ");
}

void QField::RefreshWhere(SqlStmt& str)
{
	str << pField->name().c_str() << " = " <<(pField->str ? "' ? '" : " ? ");
}

void QField::UpdateWhere(SqlStmt& str)
{
	str << pField->name().c_str() << " = " <<(pField->str ? "' ? '" : " ? ");
}

QFields& QField::operator>>(QFields& qf)
{
	qf.Add(this);
	if(next)
		return next->operator>>(qf);
	else
		return qf;
}
//void QField::SQLStr(CString& str, bool mark, bool useAlias /*= true*/)
/*{
	if(useAlias)
		str += pQTable->Alias + ".";
	str += pField->name;
	if(mark)
		pQTable->Mark(false);
}

/*QField* QField::Find(QTable* pTable, const char* str)
{
	if(pQTable == pTable && pField->name == str)
		return this;
	else
	{
		if(next)
			return next->Find(pTable, str);
		else
			return NULL;
	}
}

QField* QField::Find(QTable* pTable, Field* pf)
{
	if(pQTable == pTable && pField == pf)
		return this;
	else
	{
		if(next)
			return next->Find(pTable, pf);
		else
			return Add(new QField(pTable, pf));
	}
}
*/

//void QField::SQLStr(CString& str, bool mark, bool useAlias /*= true*/)
/*{
	if(useAlias)
		str += pQTable->Alias + ".";
	str += pField->name;
	if(mark)
		pQTable->Mark(false);
}
void QField::Find(QTable* pqt, QFields& qf)
{
	if(pQTable == pqt)
		qf.Add(this);
	if(next)
		next->Find(pqt, qf);
}

const char* QField::GetCaption()
{
	const char* s1 = pQTable->GetFCapt(pField->fnum);
	if(s1)
		return s1;
	else
		return pField->capt;
}

int QField::GetStyle()
{
	int st = pQTable->GetStyle();
	if(st)
		return st;
	else
		return pField->style;
}
*/
