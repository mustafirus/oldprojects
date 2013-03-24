/*
 * QTree.cpp
 *
 *  Created on: 13 марта 2011
 *      Author: golubev
 */

#include "stdx.h"
#include "Dict.h"

int QTable::aliasNum=0;
///////////////////////////////////////////////////////
QTable* QTable::add(QTable* pqt)
{
	if(next) return next->add(pqt);
	else{ next = pqt; pqt->next = NULL; return pqt;}
}

QTable* QTable::find(QTable* pqt, FKey* pfk)
{
	if(pPTable == pqt && pFKey == pfk)
		return this;
	if(next)
		return next->find(pqt, pfk);
	else
		return NULL;
}



QField* QTable::getQField(const string& name)
{
	Field* pf = pNTable->getField(name);
	if(!pf)
		return NULL;
	if(!pQField)
		return pQField = new QField(this, pf);
	return
		pQField->GetQField(pf);
}

QTable* QTable::getLink(QTable* pqt)
{
	PKey* ppk;
	if(pqt->pPTable)
		ppk = pqt->pFKey->pPKey;
	else
		ppk = pqt->pNTable->getPKey();
	ASSERT(ppk);
	FKey* pfk = pNTable->getFKey(ppk);
	if(!pfk)
	{
		DbError2(MSG_FK_NOT_FOUND, ppk->name, *pNTable);
		return NULL;
	}
	return join(pfk);
}

///////////////////////////////////////////////////////
QField::QField(QTable* pqt, Field* pf){
	pQTable = pqt;
	pField = pf;
	next = NULL;
}

QField::~QField(){
	if(next)delete next;
}

QField* QField::add(QField* pqf){
	if(!pqf)
		return NULL;
	if(next)
		next->add(pqf);
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
			return add(new QField(pQTable, pf));
	}
}

///////////////////////////////////////////////////////
QView::QView(Dict* pd, const char** props) : Props(props){
	pQTree = pd->getQTree(getProp("table"));
	pd->addQView(this);
}


list<string> QTree::parse(const char* s, char delimeter){
	char* str = strdup(s);
	char* strs[32];
	char* s1;
	strs[0] = s1 = str;
	int n = 1;
	while( (s1 = strchr(s1, delimeter)) ){
		*s1++ = 0;
		strs[n++] = s1;
	};
	list<string> v;
	for (int i = 0; i < n; ++i) {
		v.push_back(strs[i]);
		printf("n:%d-%s\n", i, strs[i]);
	}
	free(str);
	return v;
}
///////////////////////////////////////////////////////
