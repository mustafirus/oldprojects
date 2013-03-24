// PrintAction.h: interface for the PrintAction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRINTACTION_H__FA5495F7_0A52_11D2_A68E_204C4F4F5020__INCLUDED_)
#define AFX_PRINTACTION_H__FA5495F7_0A52_11D2_A68E_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Action.h"
#include "Set.h"
class RKey;
class RField;
class PrintAction : public Action  
{
	int			lang;
	CFile		file;
	int			left;
	int			top;
public:
	PrintAction(Action* pParent, RKey* key);
	virtual ~PrintAction();
	virtual bool Return(bool success = false){if(file.m_hFile != (UINT)CFile::hFileNull)file.Close();delete this; return true;};
	virtual void Return(Action* pa, bool success = false);

private:
	int		pnum; 
//	bool	GetPrintForm();
	RKey*	pRKey;
	CDC*	pDC;
	CFont*	pFont;
//	RFields	pRFields;
//	RECT*	rects;
//	int		fields;
	void Print();
	static int Run(PrintAction* pa);
};

#endif // !defined(AFX_PRINTACTION_H__FA5495F7_0A52_11D2_A68E_204C4F4F5020__INCLUDED_)
