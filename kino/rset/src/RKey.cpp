// RKey.cpp: implementation of the RKey class.
//
//////////////////////////////////////////////////////////////////////
#if 0

#include "stdx.h"
#include "Dict.h"
#include "QTable.h"
#include "Record.h"
//#include "RecordSet.h"
#include "SqlStmt.h"
#include "DataBase.h"
#include "Query.h"
#include "RField.h"
#include "QField.h"
#include "RKey.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
RKey::RKey(QTable* pqt, Record* psrc) :
pQTable(pqt)/*, pTRec(NULL), rest(NULL), clone(false), copy(false)*/
{
	QFields qf;
	pqt->GetKeyFields(qf);
	psrc->GetRFields(qf, pRFields);
	pQuery = Record::pDict->GetQuery(*pqt);
	pqt = *pQuery;
	pqt->GetKeyFields(pQFields);
//	pQuery->GetQFields(((Table*)*(pQuery->pQTable))->pFields,pQFields);
/*	field_num = rf.GetCount();
	pRsrc = new RField*[field_num];
	for(int i=0; i < field_num; i++)
	{
		pRsrc[i] = rf[i];
	}
*/
}

bool RKey::IsPrimary()
{
	return pQTable->IsMaster();
};

void RKey::Select(SqlStmt& str)
{
	for(int i = 0; i < pQFields.GetCount(); i++)
	{
		if(i!=0)
			str << " AND ";
		pQFields[i]->Where(str);
	}
}
void RKey::Delete(SqlStmt& str)
{
	for(int i = 0; i < pQFields.GetCount(); i++)
	{
		if(i!=0)
			str << " AND ";
		pQFields[i]->DeleteWhere(str);
	}
}
void RKey::SrcSelect(SqlStmt& str)
{
	for(int i = 0; i < pRFields.GetCount(); i++)
	{
		if(i!=0)
			str << " AND ";
		(*pRFields[i])->Where(str);
	}
}
void RKey::Update(SqlStmt& str)
{
	for(int i = 0; i < pQFields.GetCount(); i++)
	{
		if(i!=0)
			str << " AND ";
		pQFields[i]->UpdateWhere(str);
	}
}

void RKey::Refresh(SqlStmt& str)
{
	for(int i = 0; i < pQFields.GetCount(); i++)
	{
		if(i!=0)
			str << " AND ";
		str << pQTable->AliaS() << ".";
		pQFields[i]->RefreshWhere(str);
	}
}

bool RKey::IsNull()
{
	for(int i = 0; i < pRFields.GetCount(); i++)
	{
		if(pRFields[i]->state == RField::s_null)
			return true;
	}
	return false;
}

void RKey::SetNull()
{
	for(int i = 0; i < pRFields.GetCount(); i++)
	{
		pRFields[i]->state = RField::s_null;
		pRFields[i]->state = RField::s_data;
	}
}

void RKey::Get(char* str)
{
	FKey* pfk = *pQTable;
	if(pfk)
		str += sprintf(str, "%s;%s",pfk->pTable->name, pfk->name);
	else
		str += sprintf(str, "%s;%s",((Table*)*pQTable)->name,
		((Table*)*pQTable)->mPKey.name);
	for(int i = 0; i < pRFields.GetCount(); i++)
	{
		str += sprintf(str, ";%s",((const char*)*(pRFields[i])));
	}
}

bool RKey::CanSet(char*& str)
{
	Breaker s1(str, ';');
	Breaker s2(s1, ';');
	char* name;
	if(s1 != true)
		return false;
	name = ((FKey*)*pQTable) ? ((FKey*)*pQTable)->pTable->name :
	((Table*)*pQTable)->name;
	if(strcmp(str, name))
		return false;
	name = (char*)(((FKey*)*pQTable) ? ((FKey*)*pQTable)->name :
	((Table*)*pQTable)->mPKey.name);
	if(strcmp(s1, name))
		return false;
	str = s2;
	return true;
}

void RKey::Set(char* str)
{
	for(int i = 0; i < pRFields.GetCount(); i++)
	{
		Breaker s3(str,';');
		ASSERT(str);
		strcpy(pRFields[i]->GetBuf(), str);
		pRFields[i]->SetData();
		str = s3;
	}
}
void RKey::SetIdentity()
{
	for(int i = 0; i < pRFields.GetCount(); i++)
	{
		if( ((QField*)(*(pRFields[i])))->ro() )
		{
			int id = Record::pDB->GetIdentity();
			ASSERT(id>0);
			itoa(id, pRFields[i]->GetBuf(), 10);
			pRFields[i]->SetData();
			return;
		}
	}
}

RKey& RKey::operator=(RKey& rk)
{
	ASSERT(pRFields.GetCount() == rk.pRFields.GetCount());
	for(int i = 0; i < pRFields.GetCount(); i++)
	{
		strcpy(pRFields[i]->GetBuf(), *(rk.pRFields[i]));
		pRFields[i]->SetData();
		pRFields[i]->SetModified();
	}
	return *this;
}

/*const char* RKey::GetName()
{
	ASSERT(pQTable);
	FKey* pfk = (FKey*)*pQTable;
	if(pfk)
		return pfk->pPKey->name;
	else
		return ((Table*)*pQTable)->mPKey.name;
}
*/
/*RKey::~RKey()
{
	ifdel(pRtrg);
	ifdel(pTRec);
	if(clone)
		return;
	if(copy)
		for(int i = 0; i < field_num; i++)
			delete pRsrc[i];
	ifdel(pRsrc);
	if(rest)
		free((void*)rest);

}
*/
/*RKey::RKey(RKey* prk) : field_num(prk->field_num),	pRsrc(prk->pRsrc),
pRtrg(NULL),pTRec(NULL),pQTable(prk->pQTable), rest(prk->rest),
clone(true), copy(false)
{
	pRtrg = new RField*[field_num];
	memset(pRtrg, 0, sizeof(RField*)*field_num);
};
*/
/*RKey::RKey(RKey& rk) : field_num(rk.field_num), pRsrc(NULL),
pRtrg(NULL),pTRec(NULL),pQTable(rk.pQTable), rest(rk.rest),
clone(false), copy(true)
{
	pRsrc = new RField*[field_num];
	for(int i = 0; i < field_num; i++)
		pRsrc[i] = new RField(*(rk.pRsrc[i]));
	pRtrg = new RField*[field_num];
	memset(pRtrg, 0, sizeof(RField*)*field_num);
};
*/

/*void RKey::SetTarget(Record* pr)
{
	for(int i=0; i < field_num; i++)
	{
		ASSERT(!pRtrg[i]);
		pRtrg[i] = pr->GetRField(pQTable->pNTable->GetKeyField(i), pRsrc[i]);
	}
}
*/
/*Record* RKey::GetSource()
{
	return pRsrc[0]->pRec;
}
*/
/*void RKey::SaveTarget()
{
	for(int i=0; i < pRtrg; i++)
	{
		ASSERT(pRtrg[i]);
		char* trg = pRtrg[i]->GetPtr();
		char* src = pRsrc[i]->GetPtr();
		if(trg != src)
		{
			strcpy(src, trg);
		}
		pRsrc[i]->SetModified();
	}
	pRsrc[0]->GetRecord()->Refresh(this);
}
*/
/*void RKey::EmptyTarget()
{
	if(pTRec)
		pTRec->Empty();
}
*/
/*void RKey::EmptySrc()
{
	for(int i=0; i < field_num; i++)
	{
		pRsrc[i]->SetEmpty();
	}
	pRsrc[0]->GetRecord()->Refresh(this);
}
*/
/*void RKey::NewTarget()
{
	GetRecord()->New();
//	if(pTRec)
//		pTRec->New();

}
*/
/*QRestr* RKey::GetSrcRestr()
{
	return pRsrc[0]->pRec->pQRestr;
}
*/
/*bool RKey::IfKey()
{
	for(int i=0; i < field_num; i++)
	{
		ASSERT(pRtrg[i] != NULL);
		char* trg = pRtrg[i]->GetPtr();
		char* src = pRsrc[i]->GetPtr();
		if(trg != src)
		{
			if(strcmp(trg, src))
				return false;
		};
	}
	return true;
}
*/
/*void RKey::FreeTarget()
{
	for(int i=0; i < field_num; i++)
	{
		ASSERT(pRtrg[i] != NULL);
		pRtrg[i] = NULL;
	}
//	if(pTRec)
//	{
//		delete pTRec;
//		pTRec = NULL;
//	}
//
}
*/
/*Record* RKey::GetRecord()
{
	if(pTRec)
		return pTRec;
	else
		return pTRec = new Record(this);
}
*/
/*void RKey::SQLRefr(CString& str, QFields& wqf, bool useAlias)
{
	wqf[0]->SQLStr(str, false, useAlias);
	pRsrc[0]->SQLData(str) ;
	for(int i=1; i<field_num; i++)
	{
		str += " AND ";
		wqf[i]->SQLStr(str, false, useAlias);
		pRsrc[i]->SQLData(str) ;
	}
}
*/
/*void RKey::SQLStr(CString& str, bool useAlias)
{
	pRtrg[0]->SQLName(str, false, useAlias);
	pRtrg[0]->SQLData(str) ;
	for(int i=1; i<field_num; i++)
	{
		str += " AND ";
		pRtrg[i]->SQLName(str, false, useAlias);
		pRtrg[i]->SQLData(str) ;
	}
}
*/
/*bool RKey::Where(CString& str, bool bWhere)
{
	if(pTRec || !pRtrg[0])
		return bWhere;

	const bool useAlias = true;

	for(int i = 0; i<field_num; i++)
	{
		if(!bWhere)
			str+="\nWHERE ";
		else
			str+=" AND ";
		pRsrc[i]->SQLName(str, false, useAlias);
		pRtrg[i]->SQLData(str) ;
	}
	return true;
}
*/
/*Table* RKey::GetTable()
{
	return pQTable->pNTable;
}
*/
/*const char* RKey::GetRest()
{
	if(rest)
		return rest;
	else
		return pQTable->GetRest();
}
*/
/*void RKey::SetRest(const char* str)
{
	rest = strdup(str);
}

CString RKey::GetForm(int num)
{
	return pQTable->GetForm(num);
}

bool RKey::Return()
{
	return pQTable->pNTable->def_edit == 2 && pQTable->pPTable;
}

const char* RKey::GetName()
{
	ASSERT(pQTable);
	return pQTable->pNTable->GetPKey()->name;
}

const char* RKey::GetSrcData(int n //= 0
								)
{
	return (n>=0 && n<field_num) ? pRsrc[n]->GetPtr() : NULL;
}

*/
/*void RKey::GetSrcFields(char* str, RFields& rf)
{
	pRsrc[0]->pRec->GetRFields(str, rf);
}
*/
/*const char* RKey::GetFun(char* str)
{
	RFields rf;
	((RecordSet*)pRsrc[0]->pRec)->pPRec->GetRFields(str, rf);
	return rf[0]->GetPtr();
}

void RKey::SetSrcData(const char* data, int n //= 0
						)
{
	if(n>=0 && n<field_num)
	strncpy(pRsrc[n]->GetPtr(), data, pRsrc[n]->GetLen());
}

void RKey::SetSrcData(int data, int n //= 0
						)
{
	if(n>=0 && n<field_num)
	itoa(data, pRsrc[n]->GetPtr(), 10);
}

void RKey::RequerySrc()
{
	pRsrc[0]->pRec->Requery();
}

void RKey::RequeryTrg()
{
	pRtrg[0]->pRec->Requery();
}
*/
#endif
