// RecordSet.h: interface for the RecordSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECORDSET_H__670D9FD4_DFFF_11D1_A611_204C4F4F5020__INCLUDED_)
#define AFX_RECORDSET_H__670D9FD4_DFFF_11D1_A611_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Rib.h"
#include "Record.h"

#define WM_NORM_REC		(WM_USER+1)

class Query;
class DbCursor;
class QRestr;
class RSBookmark;
class RSField;

class RecordSet : public RiB, public Record
{
	DbCursor*	pCur;
	int			dbsize;
	bool		dir;

	virtual void AssistLoad(){};
public:
	RecordSet(int rows, Query* pq/*, Record* pr = NULL*/);
	virtual void SetLink(RLink* prl, RKey* prk);
	virtual ~RecordSet();

	bool	Load();
	bool	Save();

	void	Delete();
	int		SelectCount();
	bool	SelectInto(const char* tablename, bool hidden = false);
	bool	Refresh(RKey* prk){return false;};
//	void	Requery(){loaded = false; Load();MoveCur(INT_MIN);};
	bool	Normalize(bool now);
	bool	Find();
	bool	MoveCur(int rows);
	bool	IsBof(){return GetFlag(bof);}
	bool	IsEof(){return GetFlag(eof);}

	RSField* operator[](int n){return (RSField*)pRFields[n];};
protected:
	RField* CreateRField(QField* pqf, RField* parent = NULL);
private:

	//bool LoadProc(int& start, int& pstart, int size, int psize);
	//void init(int& currow, int& pstart);

	
public:
//	bool SaveToFile(const char* filename);
friend class RKey;
friend class DataBase;
friend class RSField;
friend class RSBookmark;
friend DWORD LoadRuner(RiB* pRec);
//	CWnd*		pWnd;
};

#endif // !defined(AFX_RECORDSET_H__670D9FD4_DFFF_11D1_A611_204C4F4F5020__INCLUDED_)
