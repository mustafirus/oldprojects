// TempRecord.cpp: implementation of the TempRecord class.
//
//////////////////////////////////////////////////////////////////////

#include "stdx.h"
#include "dblib.h"
#include "TempRecord.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define pDB Record::pDB
TempRecord::TempRecord(const char* str, RKey* prk) : pDbStmt(NULL)
{
	pDbStmt = pDB->Prepare(str);

	if(prk)
		pDB->BindParameter(prk);
	pDB->Exec();

	pTabInfo = new TABLEINFO;
	PCOLINFO& pci = pTabInfo->pColInfo;
	pDB->GetInfo(pTabInfo);
	buf = new char[pTabInfo->size];
	
	for(int i = 0; i < pTabInfo->cols; i++)
	{
		pDB->Bind(i+1, buf + pci[i].offset, pci[i].len, &pci[i].intr);
	}
}

TempRecord::~TempRecord()
{
	pDB->FlushEx();
	delete pTabInfo->pColInfo;
	delete pTabInfo;
	delete buf;
}

bool TempRecord::MoveNext()
{
	return pDB->Read();
}

const char* TempRecord::operator[](int n)
{
	COLINFO*& pci = pTabInfo->pColInfo;
	if(n<0 || n>=pTabInfo->cols || pci[n].intr == SQL_NULL_DATA)
		return NULL;
	return buf + pci[n].offset;
}

const char* TempRecord::operator[](const char* name)
{
	COLINFO*& pci = pTabInfo->pColInfo;
	for(int i = 0; i < pTabInfo->cols; i++)
	{
		if(!stricmp(pci[i].name, name))
			return pci[i].intr == SQL_NULL_DATA ? NULL : buf + pci[i].offset;
	}
	return NULL;
}
