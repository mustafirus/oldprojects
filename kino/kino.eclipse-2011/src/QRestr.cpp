// QRestr.cpp: implementation of the QRestr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdx.h"
//#include "ConstructException.h"
#include "Dict.h"
#include "QField.h"
#include "DbCursor.h"
#include "Record.h"
#include "RField.h"
#include "SqlStmt.h"
#include "Rkey.h"
#include "Query.h"
#include "QRestr.h"

IMPLEMENT_DICT_MEMORY(QRestr)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

char* GetRest(){return NULL;};
/*const char* RKey::GetRest()
{
	if(rest)
		return rest;
	else
		return pQTable->GetRest();
}
*/

//inline char* sdup(const char* X){ char* s1 = new char[strlen(X)+1]; strcpy(s1, X); return s1;};
Record* GetSource(){return NULL;};//	return pRsrc[0]->pRec;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
QRestr* Fun(Query* pq, const char* str, QRestr* pqr, Record* pr);
QRestr* QRestr::Create(Query* pq, RKey* prk, QRestr* pqr, Record* pr /*= NULL*/)
{
	char* s1;
	char* s2 = (char*)((QTable*)*prk)->GetRest();
	if(!s2 || !*s2)
		return pqr;
	do{
		s1 = s2;
		s2 = strchr(s1 ,';');
		if(s2)
			*s2 = 0;
		pqr = Fun(pq, s1, pqr, pr);
		if(s2)
			*s2++ = ';';
	}while(s2);
	return pqr;
}

QRestr::QRestr(QField* pqf, const char* str, RField* prf, QRestr* pqr) :
pQField(pqf), data(sdup(str)), pRField(prf), next(pqr){}

QRestr* Fun(Query* pq, const char* str, QRestr* pqr, Record* pr)/* : data("NULL"), pRField(NULL)*/
{
	char  s1[30];
	char  s2[100];
	char  s3[30];
	int n = sscanf(str, " %[^ <>=]%[^{]{%[^}]}", s1, s2, s3);
	if(n<2)
		return NULL;
	QFields qf;
	pq->GetQFields(s1, qf);
	if(qf.GetCount() < 1)
		return NULL;
	if(qf.GetCount()>1)
		return NULL;
	QField* pqf = qf[0];
	RField* prf = NULL;
	if(n == 3)
	{
		ASSERT(pr);
		RFields rf;
		pr->GetRFields(s3, rf);
		if(rf.GetCount()>1)
			return NULL;
		prf = rf[0];
	}
	return new QRestr(pqf, s2, prf, pqr);
}

//QRestr::QRestr(Query* pq, const char* str, QRestr* pqr, Record* pr) : data("NULL"), pRField(NULL)
//{
/*	char  s1[30];
	char  s2[100];
	char  s3[30];
	int n = sscanf(str, " %[^ <>=]%[^{]{%[^}]}", s1, s2, s3);
	if(n<2)
		ErrorConstruct("Error: incorrect syntax in restriction");
	QFields qf;
	pq->GetQFields(s1, qf);
	if(qf.GetCount() < 1)
		ErrorConstruct("Error: field in restriction not found");
	if(qf.GetCount()>1)
		ErrorConstruct("Error: multiple fields in restriction");
	pQField = qf[0];
	data = s2;
	if(n == 3)
	{
		RFields rf;
		pr->GetRFields(s3, rf);
		if(rf.GetCount()>1)
			ErrorConstruct("Error: multiple fields in restriction");
		pRField = rf[0];
	}
	next = pqr;
*/
//}

QRestr::~QRestr()
{
	if(next)
		delete next;
}

bool QRestr::Where(SqlStmt& str, bool bWhere)
{
	if(!bWhere)
	{
		str << "\nWHERE ";
		bWhere = true;
	}
	else
		str << " AND ";
	pQField->Select(str);
	pQField->Mark();
	str << " " << data;
	if(pRField)
		str << "?";
	if(next)
		bWhere = next->Where(str, bWhere);
	return bWhere;
}

void QRestr::Bind(DbCursor* pCur)
{
	if(pRField)
		pCur->BindParameter(pRField);
	if(next)
		next->Bind(pCur);
}

bool QRestr::SetDefaults(Record* pr)
{
	bool ret = false;
	char* s1 = data;
	bool eq = false;
	RField* prf;
	while(*s1)
	{
		if(*s1 == '=' && !eq)
			eq = true;
		else if(*s1 != ' ')
		{
			if(eq)
				goto doit;
			else
				goto next;
		}
		s1++;
	}
/*	if(!eq)
		goto next;
*/
doit:
	prf = pr->GetRField(pQField);
	ret = true;
	if(pRField)
		strcpy(prf->GetBuf(), *pRField);
	else if(s1)
		strcpy(prf->GetBuf(), s1);
	else
		ret = false;
	if(ret)
		prf->SetModified();
next:
	if(next)
		ret = next->SetDefaults(pr) || ret;
	return ret;
}

/*void QRestr::GetQFields(QFields& qf, Strs& st)
{
	char* s1 = strchr(data, '=');
	if(s1)
	{
		qf.Add(pQField);
		if(pRField)
			st.Add(pRField->GetPtr());
		else
			st.Add(s1);
	}
	if(next)
		next->GetQFields(qf, st);
}
*/
/*const char* QRestr::GetData(QFields* pqf)
{
	if(pqf == pQField)
	{
		if(pRField)
			return pRField->GetPtr();
		else
			if()
	}else if(next)
		next->GetData(QFields* pqf);
	else
		return NULL;
}
*/