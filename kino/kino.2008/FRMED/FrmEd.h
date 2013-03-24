// FrmEd.h : main header file for the FRMED application
//

#if !defined(AFX_FRMED_H__F7961D06_0A02_11D2_A68D_204C4F4F5020__INCLUDED_)
#define AFX_FRMED_H__F7961D06_0A02_11D2_A68D_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CFrmEdApp:
// See FrmEd.cpp for the implementation of this class
//

class CFrmEdApp : public CWinApp
{
public:
	CFrmEdApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFrmEdApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFrmEdApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRMED_H__F7961D06_0A02_11D2_A68D_204C4F4F5020__INCLUDED_)
