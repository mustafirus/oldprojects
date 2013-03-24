// TempRecord.h: interface for the TempRecord class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEMPRECORD_H__5998CCB3_55E5_11D2_A74B_204C4F4F5020__INCLUDED_)
#define AFX_TEMPRECORD_H__5998CCB3_55E5_11D2_A74B_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Defines.h"
class DbStmt;

class TempRecord  
{
	DbStmt*		pDbStmt;
	PTABLEINFO	pTabInfo;
	char*		buf;
public:
	TempRecord(const char* table, RKey* prk);
	virtual ~TempRecord();
	bool MoveNext();
	const char* operator[](int n);
	const char* operator[](const char* name);
};

#endif // !defined(AFX_TEMPRECORD_H__5998CCB3_55E5_11D2_A74B_204C4F4F5020__INCLUDED_)
