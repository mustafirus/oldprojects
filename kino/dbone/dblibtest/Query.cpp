// Query.cpp: implementation of the Query class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Dict.h"
#include "QTable.h"
#include "QField.h"
#include "DbException.h"
#include "Query.h"

IMPLEMENT_DICT_MEMORY(Query)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Query::Query(Table* pt)
{
	pQTable = new QTable(pt);
//	pQField = NULL;
//	next = NULL;
}
Query::~Query()
{
//	ifdel(pQField);
	ifdel(pQTable);
//	ifdel(next);
}
char* Query::GetQFields(const char* str, QFields& qf, QTable* pqt)
{
	if(!str)
		return (char*)str;
	ASSERT(str);
	str += strspn(str, " ,");
	ASSERT(*str != ' ' && *str != ',');
	if(!pqt)
		pqt = pQTable;
	char* s1 = (char*) str;
	char* s2;
	QTable* last;
	QField* pqf;
	int k;
	char ch;
	do
	{
		ch=s1[k = strcspn(s1," ,")];
		s1[k]=0;
		s2 = s1;
		pqf = NULL;
		last = pqt;
		char* s3;
		while(s3 = strchr(s2, '.'))
		{
			*s3=0;
			try{
			last = last->Join(s2);
			}catch(DbException* e)
			{
				e->Effect();
				*s3++='.';
				goto next;
			}
			*s3++='.';
			s2=s3;
		};
/*		if(!pQField)
			pf = pQField = last->CreateField(s2);
		if(!pf)
			pf = pQField->Find(last,s2);
		if(!pf)
			pf = pQField->Add(last->CreateField(s2));
*/
		pqf = last->GetQField(s2);
		if(pqf)
		{
			qf.Add(pqf);
		}else
		{
			const char* ext = NULL;
			try{
				ext = last->GetExtent(s2);
				last = last->Join(s2);
			}catch(DbException* e)
			{
				e->Effect();
				goto next;
			}
			GetQFields(ext, qf, last);
		}
next:	s1[k]=ch;
		s1+=k;
	}while(*(s1+=strspn(s1," ,")));
	return s1;
}

void Query::GetKeyFields(QTable* pqt, QFields& qtrg)
{
	pqt->GetKeyFields(qtrg);
/*	if(pQField)
		return;
	pQField = qtrg[0];
	for(int i=1; i<qtrg.GetCount(); i++)
		pQField->Add(qtrg[i]);
*/
}

/*Query::Query(const char* table, const char *str)
{
	pQTable = new QTable(table, "master");
	pQField = NULL;
	next = NULL;

	if(!str)
		return;
	char* s1 = (char*) str;
	int k;
	char ch;
	do
	{
		k = strcspn(s1," ,");
		ch=s1[k];
		s1[k]=0;
		if(*s1)
			GetQField(s1);
		s1[k]=ch;
		s1+=k+1;
	}while(*s1);
}
*/
/*Query::Query(RKey* rkey)
{
	pQTable = new QTable(rkey->GetTable(), "master");
	pQField = NULL;
	next = NULL;
}
*/
/*Query* Query::Add(Query* pq)
{
	if(next)
		return next->Add(pq);
	else
	{
		next=pq;
		return pq;
	}
}
*/

/*Query* Query::Find(Table* pt)
{
	if(pQTable->pNTable == pt)
		return this;
	else
	{
		if(next)
			return next->Find(pt);
		else
			return NULL;
	}
}
*/
/*QField* Query::GetQField(const char* str)
{
	QField* pf = NULL;
	QTable* last = pQTable;
	char* s1=(char*)str;
	char* s2;
	while(s2 = strchr(s1, '.'))
	{
			*s2=0;
			last = last->Join(s1, pQField);
			*s2++='.';
			s1=s2;
	};
	ASSERT(last);
	if(!pQField)
		return pQField = last->CreateField(s1);
	pf = pQField->Find(last,s1);
	if(!pf)
		pf = pQField->Add(last->CreateField(s1));
//	ASSERT(pf);
	return pf;
}
*/

/*void Query::GetQFields(QTable* pqt, QFields& qf, bool child)
{
	pQField->Find(pqt, qf);
	if(child)
		while(pqt = pqt->FindChild(pqt))
			GetQFields(pqt, qf, child);
}
*/

/*QField* Query::GetQField(Field* pfld, QTable* pqt)
{
	QField* pf = NULL;
	if(!pqt)
		pqt = pQTable;
	ASSERT(pqt);
	if(!pQField)
	{
//		ASSERT(0);
		return pQField = new QField(pqt, pfld);
	}
	pf = pQField->Find(pqt, pfld);
	if(!pf)
		pf = pQField->Add(new QField(pqt, pfld));
	ASSERT(pf);
	return pf;
}
*/
/*
void Query::GetNKeyFields(QTable* pqt, QFields& qtrg)
{
	pqt->GetNKeyFields(pQField, qtrg);
	if(pQField)
		return;
	pQField = qtrg[0];
	for(int i=1; i<qtrg.GetCount(); i++)
		pQField->Add(qtrg[i]);
}

QTable* Query::GetLinkTable(Query* pq)
{
	return pQTable->Join(pq->pQTable->pNTable->GetPKey()->name); // Amibigous looking for a table by key name
}
*/
Query::operator QTable()
{
	return *pQTable;
};
void sdfiuf()
{
	Query* pq=NULL;
	const char* s1 = ((Table)(QTable)*pq).name;
}