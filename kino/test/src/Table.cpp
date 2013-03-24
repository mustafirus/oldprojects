// Table.cpp: implementation of the Table class.
//
//////////////////////////////////////////////////////////////////////

#include "stdx.h"
#include "Dict.h"
#include "Field.h"
#include "Table.h"



Field* Table::GetField(int n)
{
	assert(n>=0 && n<_fields.size());
	return _fields[n];
}

Field* Table::GetField(const char* name)
{
	return _fieldm[name];
/*
	for(int i=0; i < _fields.size(); i++)
	{
		if(!strcmp(fields[i]->name, name))
			return fields[i];
	}
	return NULL;
*/
};

FKey* Table::GetFKey(const char* name)
{
	for(int i = 0; i < pFKeys.size(); i++)
	{
		if(!strcmp(name,pFKeys[i]->name))
			return pFKeys[i];
	}
	return NULL;
}

FKey* Table::GetFKey(PKey* pRef)
{
	ASSERT(pRef);
	for(int i = 0; i < pFKeys.size(); i++)
	{
		if(pFKeys[i]->pPKey == pRef)
			return pFKeys[i];
	}
	return NULL;
}

bool Table::Find(Field* pf)
{
	for(int i = 0; i < _fields.size(); i++)
		if(_fields[i]==pf)
			return true;
		return false;
}

#ifdef _DEBUG
void Table::Dump()
{
	printf("Table %s\n", name);
	mPKey.Dump();
	for(int i = 0; i < pFKeys.GetSize(); i++)
		pFKeys[i]->Dump();

}
void PKey::Dump()
{
	printf("Primary - %s\n", name);
	for(int i = 0; i < fields.GetCount(); i++)
	{
		printf("\t%s\n", fields[i]->name);
	}
}
void FKey::Dump()
{
	printf("Foreign - %s\n", name);
	printf("\tReferences - %s\n", pPKey->name);
	for(int i = 0; i < fields.GetCount(); i++)
	{
		printf("\t%s - %s\n", fields[i]->name, (*pPKey)[i]->name);
	}
}
#endif //_DEBUG
