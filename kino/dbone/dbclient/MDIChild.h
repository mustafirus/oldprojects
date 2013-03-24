// MDIChild.h: interface for the MDIChild class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MDICHILD_H__E7D87216_272F_11D2_A6D9_204C4F4F5020__INCLUDED_)
#define AFX_MDICHILD_H__E7D87216_272F_11D2_A6D9_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Wnd.h"

class MDIMain;
class MDIChild : public Wnd  
{
public:
	MDIChild(const char* classname, const char* szTitle);
	virtual ~MDIChild();
	
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{ return ::DefMDIChildProc(*this, message, wParam, lParam); }
	
	static MDIMain* getMainWnd();
};

#endif // !defined(AFX_MDICHILD_H__E7D87216_272F_11D2_A6D9_204C4F4F5020__INCLUDED_)
