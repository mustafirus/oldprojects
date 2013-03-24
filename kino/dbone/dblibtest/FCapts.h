// FCapts.h: interface for the FCapts class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FCAPTS_H__CCEE1E13_363D_11D2_A701_204C4F4F5020__INCLUDED_)
#define AFX_FCAPTS_H__CCEE1E13_363D_11D2_A701_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class FKey;
class Field;
class FCapts  
{
	FKey*	pFKey;
	Field*	pField;
	FCapts* next;
	char*	capt;
public:
	FCapts(FKey* pfk, Field* pf, const char* str, FCapts* pn): next(pn), pFKey(pfk), pField(pf)
	{
		ASSERT(lstrlen(str));
		capt = new char[lstrlen(str)+1];
		lstrcpy(capt, str);
	};
	~FCapts(){delete capt;if(next)delete next;};
	const char* GetCapt(FKey* pfk, Field* pf)
	{
		if(pFKey == pfk && pField == pf)
			return capt;
		else if(next)
			return next->GetCapt(pfk, pf);
		else return NULL;
	};
DECLARE_DICT_MEMORY
};

#endif // !defined(AFX_FCAPTS_H__CCEE1E13_363D_11D2_A701_204C4F4F5020__INCLUDED_)
