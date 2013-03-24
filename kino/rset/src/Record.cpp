// Record.cpp: implementation of the Record class.
//
//////////////////////////////////////////////////////////////////////
#if 0

#include "stdx.h"
#include "Dict.h"
#include "Query.h"
#include "QField.h"
#include "SqlStmt.h"
#include "RField.h"
#include "RKey.h"
#include "QRestr.h"
#include "RLink.h"
#include "Wait.h"
#include "DbException.h"
#include "DataBase.h"
#include "RecordSet.h"

//#define GrowBy	4

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
DataBase* Record::pDB = NULL;
Dict* Record::pDict = NULL;
UINT Record::CF_RKEY = RegisterClipboardFormat("CF_RKEY");

Record::Record() : 
pPRKey(NULL), pDbStmt(NULL),
pQuery(NULL), pQRestr(NULL), pSrcLink(NULL), pTrgLink(NULL){}

Record::Record(RKey* rkey) : 
pPRKey(rkey), pDbStmt(NULL),
pQuery(*rkey), pQRestr(NULL), pSrcLink(NULL), pTrgLink(NULL){}

Record::~Record()
{
	for(int i=0; i<pRFields.GetCount(); i++)
	{
		delete pRFields[i];
	}
	for(i=0; i<pRKeys.GetCount(); i++)
	{
		delete pRKeys[i];
	}
	while(pSrcLink)
		delete pSrcLink;
	ifdel(pQRestr);
}
/////////////////////////////////////////////////////////
RField* Record::Find(QField* pqf)
{
	ASSERT(pqf);
	for(int i=0; i<pRFields.GetCount(); i++)
	{
		if(*pRFields[i] == pqf)
			return pRFields[i];
	}
	return NULL;
}


RField* Record::CreateRField(QField* pqf, RField* parent)
{
	RField* prf = new RField(pqf, this, parent);
	if(state == s_blank)
		prf->state = RField::s_new;
	else if(state != s_prepare)
		state = s_dummy;
	pDbStmt = NULL;
	return prf;
}
/////////////////////////////////////////////////////////////
bool Record::Load()
{
	state /= s_prepare;
	ASSERT(pPRKey);
	if(pPRKey->IsNull())
		return New(), true;
	SqlStmt str;
	if(!pDbStmt)
	{
		ASSERT(pRFields.GetCount());
		str << "SELECT ";
		for(int i = 0; i<pRFields.GetCount(); i++)
		{
			if(i>0)
				str << ", ";
			(*pRFields[i])->Select(str);
			(*pRFields[i])->Mark();
		}
		str << "\nFROM ";
		pQuery->pQTable->Select(str);
		ASSERT(pPRKey);
		str << "\nWHERE ";
		pPRKey->Select(str);
	}		
	try{
		if(!pDbStmt)
		{
			pDbStmt = pDB->Prepare(str);
		}else
			pDB->Set(pDbStmt);
		pDB->BindParameter(pPRKey);
		for(int i=0; i < pRFields.GetCount(); i++)
		{
			if(pRFields[i]->state == RField::s_modified)
				continue;
			pDB->Bind(i+1, pRFields[i]);
		}
		pDB->Exec();
		if(!pDB->Read())
			return false;
		pDB->FlushEx();
	}catch(DbException* e)
	{
		e->Effect();
		pDB->FlushEx();
		return false;
	}
	for(int i = 0; i<pRFields.GetCount(); i++)
	{
		if(pRFields[i]->state != RField::s_modified)
			pDB->CheckData(pRFields[i]);
	}
	state /= s_dummy;
	return true;
}

void Record::AssistLoad()
{
	ASSERT(pPRKey);
	ASSERT(pRFields.GetCount());
	
	bool nul = true;
	RFields rf;
	RField* prf = NULL;
	if(!pPRKey->IsNull())
	{
		nul = false;
		SqlStmt str;
		str << "SELECT ";
		bool comma = false;
		int fn = 1;
		for(int i = 0; i<pRFields.GetCount(); i++)
		{
			if(pRFields[i]->state == RField::s_data)
				continue;
			if(comma)
				str << ", ";
			else
				comma = true;
			prf = pRFields[i];
			(*prf)->Select(str);
			(*prf)->Mark();
			rf.Add(prf);
		}
		if(!comma)
			return;
		str << "\nFROM ";
		pQuery->pQTable->Select(str);
		str << "\nWHERE ";
		pPRKey->Select(str);
		pDB->Prepare(str);
		for(i = 0; i<rf.GetCount(); i++)
			pDB->Bind(fn++, rf[i]);
		pDB->BindParameter(pPRKey);
		pDB->Exec();
		if(!pDB->Read())
			nul = true;
		pDB->FlushEx();
	}
	for(int i = 0; i<rf.GetCount(); i++)
	{
		if(nul)
			rf[i]->SetData(true);
		else
			pDB->CheckData(rf[i]);
	}
	state /= s_dummy;
}

bool Record::LoadDefaults()
{
	RField* prf;
	RKey* prk;
	QTable* pqt = *pQuery;
	while(pqt)
	{
		QFields qf;
		pqt->GetKeyFields(qf);
		for(int i = 0; i<qf.GetCount(); i++)
		{
			prf = Find(qf[i]);
			if(!prf || prf->state == RField::s_null)
				goto next;
		}
		prk = GetRKey(pqt);
		ASSERT(prk);
		if(!prk->IsNull())
			Refresh(prk);
next:	pqt = *pqt;
	}
	return true;
}

bool Record::Insert()
{
	ASSERT(pRFields.GetCount());
	SqlStmt str;
	QTable* qt = *pQuery;
	str << "INSERT INTO " << (const char*)*(Table*)*qt << "( ";
	QFields qf;
	RFields rf;
	*qt >> qf;
	Find(qf,rf);
	bool comma = false;
	for(int i = 0; i < rf.GetCount(); i++)
	{
		if(rf[i]->state != RField::s_modified)
			continue;
		if(comma)
			str << ", ";
		else
			comma = true;
		str << ((Field*)*(QField*)*rf[i])->name;
	}
	if(!comma)
		return false;
	str << ")\nVALUES ( ";

	comma = false;
	for(i = 0; i < rf.GetCount(); i++)
	{
		if(rf[i]->state != RField::s_modified)
			continue;
		if(comma)
			str << ", ";
		else
			comma = true;
		bool s = ((Field*)*rf[i])->str;
		str << (s ? "'" : "") << *rf[i] << (s ? "'" : "");
	}
	str << " )";

	try
	{
		pDB->ExecDirect(str);
		pDB->FlushEx();
		ASSERT(pPRKey);
		pPRKey->SetIdentity();
	}catch(DbException* e){e->Effect();return false;}

	state /= s_blank; state /= s_modified;
	ASSERT(state != s_dummy);
	for(i = 0; i < rf.GetCount(); i++)
		rf[i]->state /= RField::s_modified;
	return true;
}

bool Record::Update()
{
	ASSERT(pRFields.GetCount());
	SqlStmt str;
	QTable* qt = *pQuery;
	str << "UPDATE " << (const char*)*(Table*)*qt << "\n SET  ";
	QFields qf;
	RFields rf;
	*qt >> qf;
	Find(qf,rf);
	bool comma = false;
	for(int i = 0; i < rf.GetCount(); i++)
	{
		if(rf[i]->state != RField::s_modified)
			continue;
		if(comma)
			str << ", ";
		else
			comma = true;
/*		bool n = rf[i]->IsNull();
		bool s = ((Field*)*rf[i])->str;
		s = s && !n;
*/		str << ((Field*)*(QField*)*rf[i])->name << " = ?";
/*		str << ((Field*)*(QField*)*rf[i])->name << " = " << 
			(s ? "'" : "") << (n ? "NULL" : (const char*)*rf[i]) << (s ? "'" : "") ;
*/	}
	ASSERT(pPRKey);
	str << "\nWHERE ";
	pPRKey->Update(str);
	
	try{
		DbStmt* pst = pDB->Prepare(str);
		for(int i = 0; i < rf.GetCount(); i++)
		{
			if(rf[i]->state != RField::s_modified)
				continue;
			pDB->BindParameter(rf[i]);
		}
		pDB->BindParameter(pPRKey);
		pDB->Exec();
		pDB->FlushEx();
	}catch(DbException* e)
	{
		pDB->FlushEx();
		e->Effect();
		return false;
	}
	state /= s_modified;
	ASSERT(state != s_dummy && state != s_blank);
	for(i = 0; i < rf.GetCount(); i++)
		rf[i]->state /= RField::s_modified;
	return true;
}

bool Record::Refresh(RKey* prk)
{
	ASSERT(pRFields.GetCount());

	QTable* pqt = *prk;
	QFields qf;
	RFields rf;
	*pqt >> qf;
	Find(qf, rf);
	ASSERT(rf.GetCount()>0);
	
	if(prk->IsNull())
	{
		for(int i = 0; i < rf.GetCount(); i++)
		{
			rf[i]->state = RField::s_null;
			rf[i]->state = RField::s_data;
			rf[i]->v++;
			state = s_modified;
		}
		return true;	
	}

	SqlStmt str;
	str << "SELECT ";
	pqt->Mark(true);
	for(int i = 0; i < rf.GetCount(); i++)
	{
		if(i>0)
			str << ", ";
		(*rf[i])->Select(str);
		(*rf[i])->Mark();
	}

	str << "\nFROM ";
	pqt->Select(str);
	str << "\nWHERE ";
	prk->Refresh(str);

//	xDump << str << "\n";
	try{
		pDB->Prepare(str);
		pDB->BindParameter(prk);
		pDB->Exec();
		for(int i=0; i < rf.GetCount(); i++)
			pDB->Bind(i+1, rf[i]);
		if(!pDB->Read())
		{
			for(i = 0; i < rf.GetCount(); i++)
			{
				rf[i]->state = RField::s_null;
				rf[i]->state = RField::s_data;
				rf[i]->v++;
				state = s_modified;
			}
			return false;
		}
	}catch(DbException* e)
	{
		pDB->FlushEx();
		e->Effect();
		return false;
	}
	for(i = 0; i < rf.GetCount(); i++)
		pDB->CheckData(rf[i]);
	pDB->FlushEx();
	return true;
}

bool Record::Save()
{
	try{
	if(state != s_modified)
		return true;
	ASSERT(pPRKey);
	bool ret = false;
	if(pPRKey->IsNull())
		ret = Insert();
	else
		ret = Update();
	Load();
		return ret;
	}catch(Exception* e)
	{
		pDB->FlushEx();
		e->Effect();
		return false;
	}
}

void Record::New()
{
	state = s_blank;
	state /= s_dummy;
	state /= s_modified;
	for(int i = 0; i<pRFields.GetCount(); i++)
	{
		pRFields[i]->state = RField::s_null;
		pRFields[i]->state = RField::s_data;
		pRFields[i]->v++;
	}
	if(pSrcLink && (*pSrcLink)->SetDefaults(this))
		LoadDefaults();
}

/*void Record::GetKeyFields(QTable* pqt, RFields& rf)
{
	QFields qf;
	pqt->GetKeyFields(qf);
	GetRFields(qf, rf);
}
*/

/*RField* Record::GetRField(const char* name, QTable* last)
{
	QField* pqf = pQuery->GetQField(name);
	if(!pqf)
		return NULL;
	RField* prf = Find(pqf);
	if(prf)
		return prf;
	else 
		return pRFields.Add(CreateRField(pqf));
}
*/
char* Record::GetRFields(char* str, RFields& rf, QTable* pqt /*= NULL*/)
{
	QFields qf;
	str = pQuery->GetQFields(str, qf, pqt);
	GetRFields(qf, rf);
	for(int i = 0; i < rf.GetCount(); i++)
		rf[i]->state /= RField::s_hidden;
	return str;
}

void Record::Find(QFields& qf, RFields& rf)
{
	int n = qf.GetCount();
	if(!n)
		return;
	RField* prf;
	for(int i = 0; i < n; i++)
	{
		prf = Find(qf[i]);
		if(prf)
			rf.Add(prf);
	}
}

RField*	Record::GetRField(QField* pqf)
{
	RField* prf = Find(pqf);
	if(prf)
		return prf;
	else
		return pRFields.Add(CreateRField(pqf));
}

void Record::GetRFields(QFields& qf, RFields& rf)
{
	int n = qf.GetCount();
	if(!n)
		return;
	for(int i = 0; i < n; i++)
		rf.Add( GetRField(qf[i]));
}

Record* Record::GetLinkedRecord(char* str)
{
	while(*str == ' ')str++;
	Breaker pkstr(str, ':');
	Breaker end(pkstr, ' ');
	Breaker fkstr(pkstr, '.');
/*	char* s1 = str;
	char* s2 = pkstr;
	char* s3 = fkstr;
	char* s4 = end;
*/
	RKey* nrk = GetRKey((*str && strcmp(str, pQuery->pQTable->pNTable->mPKey.name)) ?
		pQuery->pQTable->Join(str) : NULL);
	Query* pq = pkstr ? pDict->GetQuery(pkstr) : *nrk;
	QTable* pqt = fkstr ? pq->pQTable->Join(fkstr) : pq->pQTable;
	pqt = pqt->GetLink((QTable*)*nrk);
	if(!pqt)
		return NULL;

	RLink* prl = GetLink(nrk);
	Record* pr = NULL;
	if(pqt->IsMaster())
	{
		pr = new Record(nrk);
		pr->SetLink(prl, nrk);
	}else
	{
		pr = new RecordSet(20, pq);
		RKey* prk = pr->GetRKey(pqt);
		pr->SetLink(prl, prk);
	}
	return pr;
}
RLink* Record::GetLink(char* str)
{
	while(*str == ' ')str++;
	Breaker pkstr(str, ':');
	Breaker end(pkstr, ' ');
	Breaker fkstr(pkstr, '.');
/*	char* s1 = str;
	char* s2 = pkstr;
	char* s3 = fkstr;
	char* s4 = end;
*/
	RKey* nrk = GetRKey((*str && strcmp(str, pQuery->pQTable->pNTable->mPKey.name)) ?
		pQuery->pQTable->Join(str) : NULL);
	Query* pq = pkstr ? pDict->GetQuery(pkstr) : *nrk;
	QTable* pqt = fkstr ? pq->pQTable->Join(fkstr) : pq->pQTable;
	pqt = pqt->GetLink((QTable*)*nrk);
	if(!pqt)
		return NULL;

	RLink* prl = GetLink(nrk);
	if(pqt->IsMaster())
	{
		Record* pr = new Record(nrk);
		pr->SetLink(prl, nrk);
	}else
	{
		Record* pr = new RecordSet(20, pq);
		pr->SetLink(prl, nrk);
	}
	return prl;
	
/*	if(semi != true)
		return NULL;
	Breaker sec(semi, ' ');
//	char* s1 = strlen(str) ? str : NULL;
	char* pkstr = semi;//strlen(sec) ? sec : NULL;
	QTable* pqt = *str ? pQuery->pQTable->Join(str) : NULL;
	RKey* nrk = GetRKey(pqt);
	RLink* prl = GetLink(nrk);
	if(!s2)
	if(!s2)
	{
		Record* pr = new Record(nrk);
		pr->SetLink(prl, nrk);
		return prl;
	}else
	{
		Breaker fk(s2, '.');
		if(fk == false)
		{
			Query* pq = pDict->GetQuery(s2);
			QTable* pqt = pq->pQTable->GetLink((QTable*)*nrk);
			if(!pqt->Master())
			{
				Record* pr = new RecordSet(pq);
			}
		}
	}
//	RKey* frk = s2 ? 
	return prl;
*/
}

RLink* Record::GetLink(RKey* prk)
{
	return new RLink(this, prk, pTrgLink);
}

void Record::SetLink(RLink* prl, RKey* prk)
{
	prl->SetTarget(this, prk ? prk : GetRKey() );
	new LinkList(prl, pSrcLink);
}

/*RField* Record::GetRField(Field* pf, RField* parent)
{
	QField* pqf = pQuery->GetQField(pf);
	RField* prf = Find(pqf);
	if(prf)
		return prf;
	else 
		return pRFields.Add(CreateRField(pqf, parent));
}

RField* Record::GetRField(QField* pqf, RField* parent)
{
	RField* prf = Find(pqf);
	if(prf)
		return prf;
	else 
		return pRFields.Add(CreateRField(pqf, parent));
}
*/

RKey* Record::GetRKey(QTable* pqt)
{
	if(!pqt)
		pqt = pQuery->pQTable;
/*	else
		pqt = pqt->GetFirstChild();
*/	for(int i = 0; i < pRKeys.GetCount(); i++)
	{
		if(*pRKeys[i] == pqt)
			return pRKeys[i];
	}
	RKey* prk = pRKeys.Add(new RKey(pqt, this));
	if(state == s_dummy && state != s_prepare)
		AssistLoad();
	return prk;
}

/*QTable* Record::GetLinkTable(Query* pq)
{
	return pq->GetLinkTable(pQuery);
}
*/
/*bool Record::LoadEx(RField** prf, int fn)
{
	ASSERT(fn);
	CString str="SELECT ";
	for(int i = 0; i < fn; i++)
	{
		if(i>0)
			str+=", ";
		prf[i]->SQLName(str, true, true);
	}
	str+="\nFROM ";
	pQuery->pQTable->SQLStr(str);
	RKey* prk = pPRKey ? pPRKey : GetRKey();
	ASSERT(prk);
	str+="\nWHERE ";
	prk->SQLStr(str, true);
	if(pQRestr)
		pQRestr->Where(str, true);

	pDB->ExecDirect(str);
	pDB->Bind(fn,prf);
	if(!pDB->Read())
		return false;
	pDB->Flush();
	return true;
}
*/
/*
bool Record::Insert()
{
	const bool convert = false;
	QFields qf;
	RFields rf;
	pQuery->GetQFields(pQuery->pQTable, qf, false);
	GetRFields(qf, rf);
	int fn = 0;
	RField* pID = NULL;
	for(int i = 0; i<rf.GetCount(); i++)
	{
		if(rf[i]->ro())
			pID = rf[i];
		if(!rf[i]->modified)	//rf[i]->ro() || 
			rf.SetAt(i, NULL);
		else
			fn++;
	}
	if(!fn)
		return true;

	CString str = "INSERT INTO ";
	pQuery->pQTable->SQLStr(str, false, true);
	str+="( ";
//	pQuery->pQTable->Mark(true);
	bool comma = false;
	for(i = 0; i<rf.GetCount(); i++)
	{
		if(!rf[i])
			continue;
		if(comma)
			str+=", ";
		else
			comma = true;
		rf[i]->SQLName(str, true, false, convert);
	}
	str +=" )\n VALUES( ";
	comma = false;
	for(i = 0; i<rf.GetCount(); i++)
	{
		if(!rf[i])
			continue;
		if(comma)
			str+=", ";
		else
			comma = true;
		rf[i]->SQLData(str, false);
		rf[i]->modified = false;
	}
	str += " )";
	try
	{
	pDB->ExecDirect(str);
	}catch(DbException * e)
	{
		e->Dump();
		MessageBox(NULL, "       ���������� �������� ������!         ", "������!", MB_OK|MB_ICONERROR);
	}
	pDB->Flush();
	Empty();
	if(pID)
	{
		pID->loaded = true;
		int id = pDB->GetIdentity();
		itoa(id, pID->GetPtr(), 10);
		pID->loaded = true;
	}
	loaded = false;
	Load();
	recnew = false;
	return true;
}

bool Record::Save()
{
	if(!modified)
		return true;
	if(recnew)
		return Insert();
	QTable* pqt = pQuery->pQTable;
	try
	{
		do
		{
			if(!Save(pqt))
				return false;
		}while(pqt = pqt->Next());
	}catch(DbException* e)
	{e->Dump();return false;}
	if(pPRKey)
		pPRKey->SaveTarget();
	Requery();
	
	return true;
}

bool Record::Save(QTable* pqt)
{
	ASSERT(pRFields.GetCount());

	const bool convert = false;
	QFields qf;
	RFields rf;
	pQuery->GetQFields(pqt, qf, false);
	GetRFields(qf, rf);
	int fn = 0;
	for(int i = 0; i<rf.GetCount(); i++)
	{
		if(!rf[i]->modified) ///rf[i]->ro() || 
			rf.SetAt(i, NULL);
		else
			fn++;
	}
	if(!fn)
		return true;

	CString str = "UPDATE ";
	pqt->SQLStr(str, false, true);
	str+="\nSET ";
	pqt->Mark(true);
	bool comma = false;
	for(i = 0; i<rf.GetCount(); i++)
	{
		if(!rf[i])
			continue;
		if(comma)
			str+=", ";
		else
			comma = true;
		rf[i]->SQLName(str, true, false, convert);
		rf[i]->SQLData(str);
		rf[i]->modified = false;
	}
//	str+="\nFROM ";
//	pqt->SQLStr(str, false);
	str += "\nWHERE ";
	if(pqt == pQuery->pQTable)
	{
		ASSERT(pPRKey);
		pPRKey->SQLStr(str, false);
	}else
	{
		QFields	wqf;
		pQuery->GetNKeyFields(pqt, wqf);
		GetRKey(pqt)->SQLRefr(str, wqf, false);
	}

	pDB->ExecDirect(str);
	pDB->Flush();
	modified = false;
	return true;
}

void Record::Delete()
{
	ASSERT(pRFields.GetCount());
	CString str="DELETE FROM ";
	pQuery->pQTable->SQLStr(str, false, true);
	ASSERT(pPRKey);
	str += "\nWHERE ";
	pPRKey->SQLStr(str, false);
	pDB->ExecDirect(str);
	pDB->Flush();
}
*/
void Record::SetModified()
{
	state = s_modified;
};

/*void Record::Empty()
{
	loaded = false;
	for(int i=0; i < pRFields.GetCount(); i++)
	{
		pRFields[i]->Empty();
		pRFields[i]->loaded = false;
	}
}
*/
/*void Record::GetDefaults()
{
	QFields qf;
	RFields rf;
	Strs	st;
	RKeys	rk;
	QRestr* pDefaults = pPRKey->GetSrcRestr();
	if(pDefaults)
		pDefaults->GetQFields(qf, st);
	GetRFields(qf,rf);
	for(int i=0; i < rf.GetCount(); i++)
	{
		rf[i]->New();
		strcpy(pRFields[i]->GetPtr(), st[i]);
		rk.Add(pRFields[i]->GetRKey());
	}//Then Update Linked fields
	loaded = true;
}
*/
/*void Record::New()
{
	recnew = true;
	for(int i=0; i < pRFields.GetCount(); i++)
	{
		pRFields[i]->New();
	}
}
*/

/*void Record::Requery()
{
	for(int i = 0; i<pChild.GetCount(); i++)
	{
		pChild[i]->Requery();
	}
};
*/
/*void Record::ChildRecord(Record * pr, bool set //= true
							)
{
	if(set)
		pChild.Add(pr);
	else
		pChild.Remove(pr);
}
*/
void Record::Copy(HWND hWnd)
{
	RKey* prk = pPRKey ? pPRKey : GetRKey();
	if(!prk || prk->IsNull())
		return;
	HANDLE h;
	if(!OpenClipboard(hWnd) ||
		!EmptyClipboard())
	{
		CloseClipboard();
		return;
	}
	int buffsize = 1024;
	int copied = 0;
	char* str = new char[buffsize];
	char* s1 = str;
	prk->Get(str);
	copied = strlen(str)+1;
	h = GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE, copied);
	memcpy(GlobalLock(h), str, copied);
	GlobalUnlock(h);
	copied = 0;
	SetClipboardData(CF_RKEY, h);

	for(int i = 0; i<pRFields.GetCount(); i++)
	{
		if(pRFields[i]->state == RField::s_hidden)
			continue;
		if(pRFields[i]->GetLen() + 20 >= buffsize - copied)
		{
			char* s2 = new char[buffsize += 1024];
			strcpy(s2, str);
			delete str;
			str = s2;
			s1 += copied;
		}
		int ll;
		copied += ll = sprintf(s1, "%s : %s\r\n",
			pDict->GetCapt(*(pRFields[i])), ((const char*)(*pRFields[i])));
		s1+=ll;
	}
	h = GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE, (copied+1)*2);
	MultiByteToWideChar(CP_ACP, 0, str, copied, (LPWSTR)GlobalLock(h), (copied+1)*2);
	GlobalUnlock(h);
	SetClipboardData(CF_UNICODETEXT, h);

	h = GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE, copied+1);
	memcpy(GlobalLock(h), str, copied+1);
	delete str;
	GlobalUnlock(h);
	SetClipboardData(CF_TEXT, h);
	CloseClipboard();
}

void Record::Paste(HWND hWnd)
{
	Wait w;
	if(!pPRKey || !OpenClipboard(hWnd) ||
		!IsClipboardFormatAvailable(CF_RKEY))
	{
		CloseClipboard();
		return;
	}
	char* str = (char* )GetClipboardData(CF_RKEY);
	CloseClipboard();
	if(!pPRKey->CanSet(str))
		return;
	char old[256];
	pPRKey->Get(old);
	pPRKey->Set(str);
	Load();
	char* s1 = old;
	pPRKey->CanSet(s1);
	pPRKey->Set(s1);
}

bool Record::CanCopy()
{
	RKey* prk = pPRKey ? pPRKey : GetRKey();
	return !prk->IsNull();
}

bool Record::CanPaste(HWND hWnd)
{
	if(!pPRKey || !OpenClipboard(hWnd) ||
		!IsClipboardFormatAvailable(CF_RKEY))
	{
		CloseClipboard();
		return false;
	}
	char* str = (char*)GetClipboardData(CF_RKEY);
	CloseClipboard();
	return pPRKey->CanSet(str);
}

void Record::Undo()
{
	for(int i = 0; i < pRFields.GetCount(); i++)
	{
		pRFields[i]->state /= RField::s_data;
		pRFields[i]->state /= RField::s_modified;
		
	}
	Load();
}
#endif
