// QTable.cpp: implementation of the QTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdx.h"
#include "Dict.h"
#include "Table.h"
#include "QField.h"
#include "DbException.h"
#include "DataBase.h"
#include "SqlStmt.h"
#include "QTable.h"



//DataBase* QTable::pDB=NULL;
//Table* QTable::GetTable(CString name){return pDB->GetTable(name);};

/*QJoin::QJoin(FKey* pfk) : m_nNKey(NULL), m_nPKey(NULL), fields(0), rst(NULL)
{
	if(pfk)
	{
		name = pfk->name;
		fields = pfk->fields;
		m_nPKey = new int(fields);
		m_nNKey = new int(fields);
		memcpy(m_nPKey,pfk->fkey,sizeof(int)*fields);
		memcpy(m_nNKey,pfk->rkey,sizeof(int)*fields);
		rst = pfk->rst;
		fcapt = pfk->fcapt;
	}
}

QJoin::~QJoin()
{
	delete m_nPKey;
	delete m_nNKey;
}

*/
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

QTable::QTable(Table* pt) : pNTable(pt), pPTable(NULL),
pQField(NULL), pFKey(NULL), next(NULL), mark(false), style(0)
{
	strcpy(Alias, "master");
}

QTable::QTable(QTable* pt, FKey* pfk) : pNTable(pfk->pPKey->pTable),
pPTable(pt), pQField(NULL), pFKey(pfk), next(NULL), mark(false), style(pfk->style)
{
	sprintf(Alias, "t%d", this);
	ASSERT(pfk && pt);
}

QTable::~QTable()
{
	ifdel(pQField);
	ifdel(next);
}

//////////////////////////////////////////////////////////////////////
// Construction helpers
//////////////////////////////////////////////////////////////////////

QTable* QTable::Add(QTable* pqt)
{
	if(next) return next->Add(pqt);
	else{ next = pqt; pqt->next = NULL; return pqt;}
}

QTable* QTable::Find(QTable* pqt, FKey* pfk)
{
	if(pPTable == pqt && pFKey == pfk)
		return this;
	if(next)
		return next->Find(pqt, pfk);
	else
		return NULL;
}

QTable* QTable::Join(const char* fkey)
{
	FKey* pfk = pNTable->GetFKey(fkey);
	if(!pfk)
	{
		DbError2(MSG_FK_NOT_FOUND, fkey, *pNTable);
		return NULL;
	}
	return Join(pfk);
}

QTable* QTable::Join(FKey* pfk)
{
	ASSERT(pfk);
	QTable* pqt = Find(this, pfk);
	if(pqt)
		return pqt;
	pqt = Add(new QTable(this, pfk));
	return pqt;
}

QField* QTable::GetQField(const char* name)
{
	Field* pf = pNTable->GetField(name);
	if(!pf)
		return NULL;
	if(!pQField)
		return pQField = new QField(this, pf);
	return 
		pQField->GetQField(pf);
}

void QTable::GetQFields(FieldSet& f, QFields& qf)
{
	for(int i = 0; i < f.GetCount(); i++)
	{
		if(!pQField)
			qf.Add(pQField = new QField(this, f[i]));
		else
			qf.Add(pQField->GetQField(f[i]));
	}
}

QTable* QTable::GetLink(QTable* pqt)
{
	PKey* ppk;
	if(pqt->pPTable)
		ppk = pqt->pFKey->pPKey;
	else
		ppk = &pqt->pNTable->mPKey;
	ASSERT(ppk);
	FKey* pfk = pNTable->GetFKey(ppk);
	if(!pfk)
	{
		DbError2(MSG_FK_NOT_FOUND, ppk->name, *pNTable);
		return NULL;
	}
	return Join(pfk);
}

void QTable::Mark(bool this_only)
{
	if(mark)
		return;
	mark=true;
	if(this_only)
		return;
	if(pPTable)
		pPTable->Mark(this_only);
}

void QTable::Select(SqlStmt& str, bool first /*= true*/)
{
	if(mark)
	{
		mark=false;
		if(!first)
		{
			str << " \nJOIN ";
		}
		str << pNTable->name().c_str() <<	" " << Alias << " ";
		if(!first)
		{
			for(int i = 0; i < pFKey->pFields.GetCount(); i++)
			{
				str << (i ? " AND " : " ON ") << Alias << "." << pFKey->pPKey->pFields[i]->name().c_str() <<
					" = " << pPTable->Alias << "." << 
					pFKey->pFields[i]->name().c_str();
			}
		}
		first = false;
	}
	if(next)
		next->Select(str, first);
};
void QTable::Delete(SqlStmt& str)
{
	str << pNTable->name().c_str();
};

QFields& QTable::operator>>(QFields& qf)
{
	(*pQField)>>qf;
	QTable* pqt = next;
	while(pqt)
	{
		if(pqt->pPTable == this)
			*pqt >> qf;
		pqt = pqt->next;
	}
	return qf;
};

/*QField* QTable::CreateField(const char* name)
{
	Field* pf = pNTable->GetField(name);
	if(pf)
		return new QField(this, pf);
	else
		return NULL;
}
*/
const char* QTable::GetExtent(const char* s1)
{
	FKey* pfk = pNTable->GetFKey(s1);
	if(!pfk)
	{
		DbError2(MSG_FK_NOT_FOUND, s1, *pNTable);
	}
	if(pfk->ext)
		return pfk->ext;
	return pfk->pPKey->ext;
}

void QTable::GetKeyFields(QFields& qf)
{
	if(pPTable)
	{
		pPTable->GetQFields(pFKey->pFields, qf);
	}else
	{
		GetQFields(((PKey*)*pNTable)->pFields, qf);
	}
}

const char* QTable::GetRest()
{
/*	if(pPTable && join.rst)
	{
		if(join.rst[strspn(join.rst," ")])
			return join.rst;
	}

*/
	if(pFKey)
		return pFKey->rst;
	return pNTable->mPKey.rst;
}

void QTable::GetNames(PNAMEINFO pn)
{
	pn->fnum = max(pNTable->mPKey.fnum, pFKey ? pFKey->fnum : 0);
	pn->pnum = max(pNTable->mPKey.pnum, pFKey ? pFKey->pnum : 0);
	pn->names = new PNAME[pn->fnum + pn->pnum];
	int i = 0;
	for(i = 0; i < pn->fnum; i++)
	{
		if(pFKey && i < pFKey->fnum && pFKey->forms[i])
			pn->names[i] = pFKey->forms[i];
		else if(i < pNTable->mPKey.fnum && pNTable->mPKey.forms[i])
			pn->names[i] = pNTable->mPKey.forms[i];
		else
			pn->names[i] = NULL;
	}
	for(int j = 0; j < pn->pnum; i++, j++)
	{
		if(pFKey && j < pFKey->pnum && pFKey->procs[j])
			pn->names[i] = pFKey->procs[j];
		else if( j <pNTable->mPKey.pnum && pNTable->mPKey.procs[j])
			pn->names[i] = pNTable->mPKey.procs[j];
		else
			pn->names[i] = NULL;
	}
}

const char* QTable::GetName(int num, int type /*= 0*/)
{
	switch(type)
	{
	case 0:
		if(pFKey)
			return (num>=0 && num<pFKey->fnum) ? pFKey->forms[num] : NULL;
		else
			return (num>=0 && num<pNTable->mPKey.fnum) ? pNTable->mPKey.forms[num] : NULL;
	case 1:
		if(pFKey)
			return (num>=0 && num<pFKey->pnum) ? pFKey->procs[num] : NULL;
		else
			return (num>=0 && num<pNTable->mPKey.pnum) ? pNTable->mPKey.procs[num] : NULL;
	default:
		return NULL;
	}
}

/*QTable* QTable::GetLink(QTable* pqtlink, QTable* pqtthis )
{
	if(!pqtthis)
		pqtthis = this;
	if(pqtthis == this && pFKey && pFKey->pPKey == &pqtlink->pNTable->mPKey)
		return this;
	if(next)
		return GetLink(pqtlink, pqtthis);
	else
		return NULL;
}
*/
/*Table* QTable::GetLink(const char* fkey)
{
	FKey* pFKey = pNTable->GetFKey(fkey);
	if(!pFKey)
		return NULL;
	return GetTable(pFKey->ref_key);
}


QTable* QTable::FindChild(QTable* pqt)
{
	if(pPTable == pqt)
		return this;
	else
		if(next)
			return next->FindChild(pqt);
		else
			return NULL;
}


CString QTable::GetForm(int n)
{
	CString str;
	if(pPTable)
	{
		if(!n)
			n=pNTable->def_edit ? pNTable->def_edit : 2; // default - list
		str = pDB->GetForm(pPTable->pNTable->name, join.name, n);
	}
	if(!str.GetLength())
	{
		if(!n)
			n=1; // default - form
		str = pDB->GetForm(pNTable->name, pNTable->pPKey->name, n);
	}
	ASSERT(str.GetLength());
	return str;
}



void QTable::GetNKeyFields(QField* pqfroot, QFields& qtrg)
{
	for(int i = 0; i < pNTable->pPKey->fields; i++)
	{
		if(pqfroot)
			qtrg.Add(pqfroot->Find(this, pNTable->pFields[pNTable->pPKey->key[i]]) );
		else
			qtrg.Add(new QField(this, pNTable->pFields[pNTable->pPKey->key[i]]) );
	}
}

int QTable::GetStyle()
{
	if(style>0)
		return style;
	else if(pPTable)
		return pPTable->GetStyle();
	else
		return 0;
}

const char* QTable::GetFCapt(int n)
{
	if(pPTable)
		return join.fcapt[n];
	else
		return NULL;
}
*/
