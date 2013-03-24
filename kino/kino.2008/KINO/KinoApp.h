// KinoApp.h: interface for the KinoApp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KINOAPP_H__A3AAAC46_4634_11D2_A720_204C4F4F5020__INCLUDED_)
#define AFX_KINOAPP_H__A3AAAC46_4634_11D2_A720_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Application.h"

class KinoApp : public Application  
{
public:
	KinoApp(){};
	virtual ~KinoApp(){};
	bool InitInstance();
	int ExitInstance(int code);
//	bool PreTranslateMessage(MSG* pMsg);
};

#endif // !defined(AFX_KINOAPP_H__A3AAAC46_4634_11D2_A720_204C4F4F5020__INCLUDED_)
