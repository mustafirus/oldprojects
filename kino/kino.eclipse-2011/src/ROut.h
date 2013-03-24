// ROut.h: interface for the ROut class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROUT_H__15077E02_3E4C_11D2_A719_204C4F4F5020__INCLUDED_)
#define AFX_ROUT_H__15077E02_3E4C_11D2_A719_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class RecordSet;
class Record;
class RField;

class ROut  
{
	HANDLE	hFile;
	DWORD NumberOfBytesWritten;
public:
	ROut(const char* filename);
	~ROut();
	ROut& operator<<(RecordSet* rec);
	ROut& operator<<(Record* pr);
	ROut& ROut::operator<<(RField* rf)
	{
		*this << (const char*)*rf;
		return *this;
	};
	ROut& ROut::operator<<(const char* str)
	{
		WriteFile(hFile, str, strlen(str), &NumberOfBytesWritten, NULL);
		return *this;
	};
};

#endif // !defined(AFX_ROUT_H__15077E02_3E4C_11D2_A719_204C4F4F5020__INCLUDED_)
