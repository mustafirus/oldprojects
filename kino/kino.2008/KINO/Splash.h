// CG: This file was added by the Splash Screen component.

#ifndef _SPLASH_SCRN_
#define _SPLASH_SCRN_

#include "WndLib.h"
// Splash.h : header file
//

/////////////////////////////////////////////////////////////////////////////
//   Splash Screen class

class SplashWnd : public Wnd
{
// Construction
protected:
	SplashWnd();

// Attributes:
public:
	HBITMAP bitmap;

// Operations
public:
	static void EnableHide();
	static void ShowSplashScreen(Wnd* pParentWnd = NULL);
//	static bool PreTranslateAppMessage(MSG* pMsg);
	virtual bool PreTranslateMessage(MSG* pMsg);

// Implementation
public:
	~SplashWnd();

protected:
	bool Create(Wnd* pParentWnd = NULL);
	void HideSplashScreen();
	static SplashWnd* c_pSplashWnd;

// Generated message map functions
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	bool OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnPaint();
	void OnTimer(UINT nIDEvent);

//	DECLARE_MESSAGE_MAP()
};


#endif
