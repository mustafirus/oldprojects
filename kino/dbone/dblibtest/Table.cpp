// Table.cpp: implementation of the Table class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Dict.h"
#include "Field.h"
#include "Table.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DICT_MEMORY(Table)
IMPLEMENT_DICT_MEMORY(PKey)
IMPLEMENT_DICT_MEMORY(FKey)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


Field* Table::GetField(int n)
{
	if(n>=0 && n<pFields.GetSize())	return pFields[n];
	else return NULL;
}

Field* Table::GetField(const char* name)
{
	for(int i=0; i < pFields.GetSize(); i++)
	{
		if(!lstrcmpi(pFields[i]->name, name))
			return pFields[i];
	}
	return NULL;
};

FKey* Table::GetFKey(const char* name)
{
	for(int i = 0; i < pFKeys.GetSize(); i++)
	{
		if(!lstrcmpi(name,pFKeys[i]->name))
			return pFKeys[i];
	}
	return NULL;
}

FKey* Table::GetFKey(PKey* pRef)
{
	ASSERT(pRef);
	for(int i = 0; i < pFKeys.GetSize(); i++)
	{
		if(pFKeys[i]->pPKey == pRef)
			return pFKeys[i];
	}
	return NULL;
}

bool Table::Find(Field* pf)
{
	for(int i = 0; i < pFields.GetSize(); i++)
		if(pFields[i]==pf)
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
	for(int i = 0; i < pFields.GetCount(); i++)
	{
		printf("\t%s\n", pFields[i]->name);
	}
}
void FKey::Dump()
{
/*	printf("Foreign - %s\n", name);
	printf("\tReferences - %s\n", pPKey->name);
	for(int i = 0; i < pFields.GetCount(); i++)
	{
		printf("\t%s - %s\n", pFields[i]->name, (*pPKey)[i]->name);
	}
*/
}
#endif //_DEBUG
