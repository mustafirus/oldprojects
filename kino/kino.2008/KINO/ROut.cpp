// ROut.cpp: implementation of the ROut class.
//
//////////////////////////////////////////////////////////////////////

#include "stdx.h"
#include "Dict.h"
#include "RField.h"
#include "RecordSet.h"
#include "ROut.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ROut::ROut(const char* filename)
{
	hFile = CreateFile(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);

}

ROut::~ROut()
{
	CloseHandle(hFile);
}
ROut& ROut::operator<<(RecordSet* pr)
{
	while(pr->MoveCur(1))
	{
		*this << (Record*)pr;
	}
	return *this;
}

ROut& ROut::operator<<(Record* pr)
{
	RFields& rf = *pr;
	int n = rf.GetCount() - 1;
	for(int i = 0; i < n; i++)
	{
		if(rf[i]->hidden())
			continue;
		*this << rf[i] << "\t";
	}
	*this << rf[n] << "\n";
	return *this;
}



