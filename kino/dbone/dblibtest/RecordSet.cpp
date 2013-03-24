// RecordSet.cpp: implementation of the RecordSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Dict.h"
#include "RSField.h"
#include "DbCursor.h"
#include "Query.h"
#include "QTable.h"
#include "RLink.h"
#include "List.h"
#include "RKey.h"
#include "DbException.h"
#include "DataBase.h"
#include "SqlStmt.h"
//#include "WaitCursor.h"
#include "RecordSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define BUFSIZE		10
//#define DBBUFSIZE	50
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
RecordSet::RecordSet(int rows, Query* pq/*, Record* pr  = NULL*/) :
RiB(rows, rows), pCur(NULL), dbsize(BUFSIZE) /*pPRec(pr), pBM(NULL)*/
{
	pQuery	=	pq;
	ASSERT(pRKeys.GetCount() == 0);
	RKey* prk = new RKey(pQuery->pQTable, this);
	pRKeys.Add(prk);
	pQRestr = QRestr::Create(pQuery, prk, pQRestr);
}

void RecordSet::SetLink(RLink* prl, RKey* prk)
{
	Record::SetLink(prl, prk);
	pQRestr = prl->CreateQRestr(pQuery, pQRestr);
}
/*RecordSet::RecordSet(int rows, char* table) :
RiB(rows, rows), pCur(NULL), dbsize(BUFSIZE), pPRec(NULL)
{
//	size	=	BUFSIZE;
	pQuery	=	pDict->GetQuery(table);
//	pQRestr = QRestr::Create(pQuery, GetRKey(), pQRestr);
//	loaded	=	true;
//	key		=	new RKey(pQuery->pQTable, this);
//	loaded	=	false;
}
*/
/*RecordSet::RecordSet(int rows, RKey* rkey) :
RiB(rows, rows), pCur(NULL), dbsize(BUFSIZE), pPRec(NULL)
{
//	pQuery	=	pDict->GetQuery(rkey);
//	size	=	BUFSIZE;
	ASSERT(rkey);
	pPRKey = rkey;
//	pPRKey->SetTarget(this);
//	pQRestr = QRestr::Create(pQuery, pPRKey, pQRestr);
//	loaded	=	true;
//	key		=	new RKey(pQuery->pQTable, this);
//	loaded	=	false;
}
*/
RecordSet::~RecordSet()
{
	ifdel(pCur);
//	ifdel(pBM);
/*	if(pPRec)
		pPRec->ChildRecord(this, false);
*/
/*	if(restr)
	{
		while(restr->next)
			delete restr->next;
		delete restr;
	}
*/
//	ifdel(key);
}

RField* RecordSet::CreateRField(QField* pqf, RField* parent)
{
	return new RSField(pqf, this, parent);//, RField* parent
}

/*RKey* RecordSet::GetRKey()
{
	return key;
}
*/
bool RecordSet::Save()
{
	if(!pSrcLink)
		return false;
	bool ret = false;
	LinkList* pList = pSrcLink;
	do{
		ret |= (*pList)->ReturnToSrc();
	}while(pList = *pList);
	return ret;
};

bool RecordSet::Load()
{
	if(state == s_loading)
		return false;
	state = s_loading;
	try
	{
		SqlStmt str;
		str << "SELECT ";
		ASSERT(pRFields.GetCount());
		for(int i = 0; i<pRFields.GetCount(); i++)
		{
			if(i>0)
				str<<", ";
			(*pRFields[i])->Select(str);
			(*pRFields[i])->Mark();
		}
		str << "\nFROM ";
		pQuery->pQTable->Select(str);
		bool bWhere = false;
		for(i = 0; i<pRFields.GetCount(); i++)
		{
			bWhere = ((RSField*)pRFields[i])->Where(str, bWhere);
		}
		
		if(pQRestr)
			bWhere = pQRestr->Where(str, bWhere);
		
		if(pSrcLink)
		{
			LinkList* pList = pSrcLink;
			do{
				(*pList)->Select(str, bWhere);
			}while(pList = *pList);
		}
		if(pCur)
		{
			pCur->SetStmt(str);
			RestoreSizes();
		}else
			pCur = pDB->GetCursor(str, dbsize);
		for(i = 0; i<pRFields.GetCount(); i++)
		{
			pCur->BindParameter((RSField*)pRFields[i]);
		}
		
		if(pQRestr)
			pQRestr->Bind(pCur);
		
		if(pSrcLink)
		{
			LinkList* pList = pSrcLink;
			do{
				(*pList)->Bind(pCur);
			}while(pList = *pList);
		}
		pCur->Execute();
		RSField* prf;
		for(i=0; i< pRFields.GetCount(); i++)
		{
			prf = (RSField*)pRFields[i]; 
			pCur->Bind(i+1, prf);
		}
		
		dir = false;
		state = s_dummy;
		Normalize(false);
		
	}catch(DbException* e)
	{
/*		state /= s_dummy;
		for(int i=0; i< pRFields.GetCount(); i++)
		{
			RSField* prf;
			prf = (RSField*)pRFields[i]; 
			prf->state = RField::s_data ;
		}
*/		e->Effect();
		pCur->SetStmt(NULL);
		state /= s_loading;
		return false;
	};
	state /= s_loading;
	return true;
}

void RecordSet::Delete()
{
	RKey* prk = GetRKey();
	if(prk->IsNull())
		return;
	SqlStmt str;
	str << "DELETE FROM ";
	pQuery->pQTable->Delete(str);
	str << "\nWHERE ";
	prk->Delete(str);
	try{pDbStmt = pDB->Prepare(str);}catch(DbException* e){e->Effect();return;}
	try
	{
		pDB->BindParameter(prk);
		pDB->Exec();
		pDB->FlushEx();
	}catch(DbException* e){e->Effect();return;}
}

bool RecordSet::SelectInto(const char* tablename, bool hidden)
{
	SqlStmt str;
	str << "SELECT ";
	ASSERT(pRFields.GetCount());
	bool comma = false;
	for(int i = 0; i<pRFields.GetCount(); i++)
	{
		if(!hidden)
			if(pRFields[i]->state == RField::s_hidden)
				continue;
		if(comma) str<<", "; else comma = true;
		(*pRFields[i])->Select(str);
		(*pRFields[i])->Mark();
	}
	str << "\n INTO " << tablename << "\nFROM ";
	pQuery->pQTable->Select(str);
	bool bWhere = false;
	for(i = 0; i<pRFields.GetCount(); i++)
	{
		bWhere = ((RSField*)pRFields[i])->Where(str, bWhere);
	}
	if(pSrcLink)
	{
		LinkList* pList = pSrcLink;
		do{
			(*pSrcLink)->Select(str, bWhere);
			(*pList)->Bind(pCur);
//			pDB->BindParameter((*pList)->GetSrcKey());
		}while(pList = *pList);
	}
	try{
	pDB->ExecDirect(str);
	}catch(DbException* e){e->Effect();return false;}
	pDB->FlushEx();
	return true;
}

int	RecordSet::SelectCount()
{
	SqlStmt str;
	str << "SELECT count(*)";
	for(int i = 0; i<pRFields.GetCount(); i++)
	{
		(*pRFields[i])->Mark();
	}
	str << "\nFROM ";
	pQuery->pQTable->Select(str);
	bool bWhere = false;
	for(i = 0; i<pRFields.GetCount(); i++)
	{
		bWhere = ((RSField*)pRFields[i])->Where(str, bWhere);
	}
	if(pSrcLink)
	{
		LinkList* pList = pSrcLink;
		do{
			(*pList)->Select(str, bWhere);
//			pDB->BindParameter((*pList)->GetSrcKey());
		}while(pList = *pList);
	}
	pDB->Prepare(str);
	for(i = 0; i<pRFields.GetCount(); i++)
	{
		pDB->BindParameter((RSField*)pRFields[i]);
	}
	if(pSrcLink)
	{
		LinkList* pList = pSrcLink;
		do{
			(*pList)->Bind(pDB);
//			pDB->BindParameter((*pList)->GetSrcKey());
		}while(pList = *pList);
	}

	pDB->Exec();
	if(!pDB->Read())
		return -1;
	int count;
	pDB->GetData(1, count);
	pDB->FlushEx();
	return count;

}
/*
bool RecordSet::Save()
{
	if(!pPRKey)
		return false;
	if(!modified)
		return true;
	pPRKey->SaveTarget();
	modified = false;
	return true;
}

void RecordSet::Delete()
{
	ASSERT(pRFields.GetCount());
	CString str="DELETE FROM ";
	pQuery->pQTable->SQLStr(str, false, true);
	str += "\nWHERE ";
	RKey* pKey = GetRKey();

	Record* pTRec = pKey->GetTarget();
	if(pTRec)
		pKey->FreeTarget();
	pKey->SetTarget(this);
	pKey->SQLStr(str, false);
	pKey->FreeTarget();
	if(pTRec)
		pKey->SetTarget(pTRec);
	try
	{
	pDB->ExecDirect(str);
	}catch(DbException * e)
	{
		e->Dump();
		MessageBox(NULL, "       Удалить нельзя!         ", "Ошибка!", MB_OK|MB_ICONERROR);
	}
	pDB->Flush();
	MoveCur(INT_MIN);
}
*/
//bool RecordSet::Refresh(RKey* prk)
//{
/*	if(modified)
	{
		loaded = false;
		return Load();
	}
	else
*///		return pCur->Fetch(FETCH_RELATIVE, 0, max(min(dbsize, GetSize()), 1) ) > 0;
//}

bool RecordSet::MoveCur(int rows)
{
	if(rows == INT_MAX || rows == INT_MIN)
	{
//		loaded = false;
		dir = (rows>0) ? true : false;
		Normalize(false);
		return true;
	}
	if(!RiB::MoveCurRow(rows))
		return false;
	return true;
//	return modified = true;
}

bool RecordSet::Normalize(bool now)
{
//	if(now)
		return LoadRuner(this) == 1;
//	else /*if(!loaded)*/
//			pWnd->PostMessage(WM_NORM_REC, (WPARAM)this, NULL);
	return false;
/*	DWORD ThreadId;
	HANDLE htr;
	htr = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE )LoadRuner,
		this, 0, &ThreadId);
	if(!htr)
		throw new Exception("Can't create thread");
	CloseHandle(htr);
*/
};

bool RecordSet::LoadProc(int& start, int& pstart, int size, int psize)
{
	int off = pstart - (size - psize)/2;
	if(dir != (off > 0))
	{
		if(dir = (off > 0))
			pCur->Fetch(DbCursor::f_relative, size, 1);
		else
			pCur->Fetch(DbCursor::f_relative, 1 - size, 1);
	}
	off = abs(off);
	DbCursor::FetchType	fetchtype;
	int	nfetchrows;
	int	rowsnfetched;
	ResetFlag(all);
	fetchtype = dir ? DbCursor::f_next : DbCursor::f_prev;
	int total = 0;
	while(off)
	{
		nfetchrows = min(dbsize, off) ;
		rowsnfetched=pCur->Fetch(fetchtype, 0, nfetchrows);
		if(rowsnfetched < nfetchrows)
			SetFlag(dir ? eof : bof);
		if(!rowsnfetched)
			break;
		off -= rowsnfetched;
		total+=rowsnfetched;
		ASSERT(off>=0);
		for(int i = 0; i < pRFields.GetCount(); i++)
		{
			if(dir)
			{
				if(start + rowsnfetched <= size)
				{
					((RSField*)pRFields[i])->Copy(start, 0, rowsnfetched);
				}else
				{
					int ct = size - start;
					((RSField*)pRFields[i])->Copy(start, 0, ct);
					((RSField*)pRFields[i])->Copy(0, ct, rowsnfetched - ct);
				}
			}else
			{
				if(start - rowsnfetched >= 0)
				{
					((RSField*)pRFields[i])->Copy(start - rowsnfetched, 0, rowsnfetched);
				}else
				{
					int ct = rowsnfetched - start;
					((RSField*)pRFields[i])->Copy(0, ct, start);
					((RSField*)pRFields[i])->Copy(size - ct,
						0, ct);
				}
			}
			
		}
		start += rowsnfetched * (dir ? 1 : -1);
		if(start < 0)
			start +=size;
		else if(start >= size)
			start -=size;
		pstart -= rowsnfetched * (dir ? 1 : -1);
		if(total >= GetPageSize())
		{
			total = 0;
		}
	}
	bool ret = true;
	if(pstart < 0)
	{
		ret = false;
		pstart = 0;
	}
	else if(pstart > size - psize)
	{
		ret = false;
		pstart = size - psize;
	}
//	pWnd->UpdateWindow();
	return ret;
}
DWORD LoadRuner(RiB* pRec)
{
//	WaitCursor cur;
	if(!((RecordSet*)pRec)->IsDummy())
		return ((RecordSet*)pRec)->LoadProc(pRec->start, pRec->pstart, pRec->size, pRec->psize) ? 1 : 0;
	else
	{
		pRec->start = 0;
		((RecordSet*)pRec)->init(pRec->currow, pRec->pstart);
		return 1;
	}
	return 1;
}

void RecordSet::init(int& currow, int& pstart)
{
	if(!dir){currow = 0;pstart = 0;}else
	{currow = GetPageSize() - 1;pstart = GetSize() - GetPageSize();}
	SetFlag(dir ? eof : bof);
	ResetFlag(dir ? bof : eof);
	state /= s_dummy;
	RestoreSizes();
	int nfetchrows ;
	int rowsfetched;
	DbCursor::FetchType fetchtype = dir ? DbCursor::f_last : DbCursor::f_first;
	dir = !dir;
	int left = GetSize();
	int start = dir ? 0 : GetSize();
	int total = 0;
	bool updated = false;
	while(left)
	{
		nfetchrows = min(dbsize,left);
		rowsfetched = pCur->Fetch(fetchtype, 0, nfetchrows);
		if(!dir) start -= rowsfetched;
		for(int i = 0; i < pRFields.GetCount(); i++)
			((RSField*)pRFields[i])->Copy(start, 0, rowsfetched);
		if(dir) start += rowsfetched;
		total += rowsfetched;
		left -= rowsfetched;
		if(rowsfetched < nfetchrows)
		{
			SetSize(total);
			SetFlag(dir ? eof : bof);
			if(!dir)
				for(int i = 0; i < pRFields.GetCount(); i++)
					((RSField*)pRFields[i])->Move(GetSize() - total, total);

				if(dir){currow = 0;pstart = 0;}else
				{currow = GetPageSize() - 1;pstart = GetSize() - GetPageSize();}
				return;
		}
		fetchtype = dir ? DbCursor::f_next : DbCursor::f_prev;
		nfetchrows = dir ? min(dbsize,left) : 1;
		if(!updated && total >= GetPageSize())
		{
			updated = true;
//			pWnd->UpdateWindow();
		}
	}
}

bool RecordSet::Find()
{
/*	if(!pPRKey)
		return false;
	MoveCur(INT_MIN);
	bool ret;
	while(!pPRKey->IfKey() && ret)
		 ret = MoveCur(1);
	if(ret)
		SetCurPos();
	return ret;
*/		return false;
}

/*bool RecordSet::SaveToFile(const char * filename)
{
	const bool useAlias = true;
	CString str="SELECT ";
	ASSERT(pRFields.GetCount());
	CString alias;
	for(int i = 0; i<pRFields.GetCount(); i++)
	{
		if(i>0)
			str+=", ";
		alias.Format("Col%d", i);
		str += alias +" = ";
		pRFields[i]->SQLName(str, true, true);
	}
	str = str + " INTO tempdb.." + filename;
	str+="\nFROM ";
	pQuery->pQTable->SQLStr(str);
	bool bWhere = false;
	for(i = 0; i<pRFields.GetCount(); i++)
	{
		bWhere = ((RSField*)pRFields[i])->Where(str, bWhere);
	}
	if(pQRestr)
		bWhere = pQRestr->Where(str, bWhere);
	for(i = 0; i < pRKeys.GetCount(); i++)
	{
		bWhere = pRKeys[i]->Where(str, bWhere);
	}
	try{
		pDB->ExecDirect(str);
		pDB->Flush();
		pDB->SaveToFile(filename, pRFields.GetCount());
		alias = "drop table tempdb..";alias += filename;
		pDB->ExecDirect(alias);
	}catch(DbException*)
	{
		return false;
	}
	return true;
}
*/