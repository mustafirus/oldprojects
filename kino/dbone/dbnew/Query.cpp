#include "StdAfx.h"
#include ".\query.h"
#include "string.h"

QField* QField::add(QField* pqf)
{
	if(!pqf)
		return NULL;
	if(_next)
		_next->add(pqf);
	else
		_next=pqf;
	return pqf;
}

QField* QField::getQField(Field* pf)
{
	if(_pField == pf)
		return this;
	else
	{
		if(_next)
			return _next->getQField(pf);
		else
			return add(new QField(_pQTable, pf));
	}
}

QField* QTable::getQField(string& name)
{
	Field* pf = _pNTable->getField(name);
	if(!pf){
		QTable* last = join(name);
		if(!last)
			return NULL;
		return last->getQField(string());
	}

	if(!_pQField)
		return _pQField = new QField(this, pf);
	return _pQField->getQField(pf);
}

QField* Query::getQField(string s)
{
	typedef list<string>	strlist;
	strlist l(s, '.');
	strlist* pl = &l;
	QTable* last = _pQTable;
	do{
		strlist* next = pl->next();
		if(next){
			last = last->join(*pl);
		}else{
			return last->getQField(*pl);;
		}
		pl = next;
	}while(pl);
	return NULL;
}

QTable* QTable::join(string& fkey)
{
	FKey* pfk = _pNTable->getFKey(fkey);
	if(!pfk)
	{
//		throw 1;
//		DbError2(MSG_FK_NOT_FOUND, fkey, *pNTable);
		return NULL;
	}
	return join(pfk);
}

QTable* QTable::join(FKey* pfk)
{
	ASSERT(pfk);
	QTable* pqt = find(this, pfk);
	if(pqt)
		return pqt;
	return add(new QTable(this, pfk));
}

QTable* QTable::add(QTable* pqt)
{
	if( _next ) return _next->add(pqt);
	else{ _next = pqt; return pqt;}
}

QTable* QTable::find(QTable* pqt, FKey* pfk)
{
	if( _pPTable == pqt && _pFKey == pfk)
		return this;
	if( _next )
		return _next->find(pqt, pfk);
	else
		return NULL;
}
