// Rib.h: interface for the Rib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RIB_H__785A9FA2_EFBB_11D1_A636_204C4F4F5020__INCLUDED_)
#define AFX_RIB_H__785A9FA2_EFBB_11D1_A636_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define EO_FLAG		1
#define BO_FLAG		2

class RiB;
DWORD LoadRuner(RiB* pRec);
class RiB
{
friend DWORD LoadRuner(RiB* pRec);
friend 	bool Normalize();
	int start;
	int size;
	int pstart;
	int psize;
	int currow;

	int size_d;
	int psize_d;
	char flag;
protected:
	enum Flag{all, eof, bof};

	RiB(int psz,int sz = 0)
	: size_d((sz > 0) ? sz : psz * 3), psize_d(min(psz,size_d)),
		start(0), size(0), pstart(0), psize(0), currow(0), flag(0)
	{
		size = size_d;		
		psize = psize_d;		
	};
	~RiB(){};

	int  GetSize(){return size;};
	void SetSize(int s){size = s; psize = min(GetPageSize_D(),size);};

	int  GetPageRow(int row){if(row<0 || row>=psize) return -1;int rr = start + pstart + row; return (rr<size) ? rr : rr-size;};
	int  GetCurRow(){int rr = start + pstart + currow; if(size == 0) return -1; return (rr<size) ? rr : rr-size;};

	bool MoveCurRow(int rows);
	
	virtual bool Normalize(bool now)=0;

	virtual int  GetPageSize_D(){return psize_d;};
	virtual int  GetSize_D(){return size_d;};
	virtual void RestoreSizes(){size = GetSize_D(); psize = GetPageSize_D();}
	virtual bool GetFlag(Flag st);
	virtual void SetFlag(Flag st);
	virtual void ResetFlag(Flag st);

public:
	int	 GetPageCur(){return currow;};
	void SetPageCur(int row){if(row<0 || row>=psize) currow = 0; currow = row;};

	int  GetPageSize(){return psize;};
	void SetPageSize(int s){ASSERT(s>=0 /*&& s<size*/);psize = min(s,size);};
};

#endif // !defined(AFX_RIB_H__785A9FA2_EFBB_11D1_A636_204C4F4F5020__INCLUDED_)
