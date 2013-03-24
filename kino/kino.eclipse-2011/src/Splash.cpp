
#include "stdx.h"
#include "resource.h"

#include "Splash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define BASECLASS Wnd

/////////////////////////////////////////////////////////////////////////////
//   Splash Screen class
SplashWnd* SplashWnd::c_pSplashWnd;

SplashWnd::SplashWnd(){}

SplashWnd::~SplashWnd()
{
	// Clear the static window pointer.
	ASSERT(c_pSplashWnd == this);
	c_pSplashWnd = NULL;
	DeleteObject(bitmap);
}

LRESULT SplashWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	DISPATCH
		ON_MSG(WM_CREATE, OnCreate)
		ON_MSG(WM_PAINT, OnPaint)
		ON_MSG(WM_TIMER, OnTimer)
	BASEROUGHT
}

void SplashWnd::EnableHide()
{
	SetTimer(*c_pSplashWnd, 1, 1000, NULL);
}

void SplashWnd::ShowSplashScreen(Wnd* pParentWnd /*= NULL*/)
{
	if (c_pSplashWnd != NULL)
	{
		c_pSplashWnd->UpdateWindow();
		return;
	}
	
	// Allocate a new splash screen, and create the window.
	c_pSplashWnd = new SplashWnd;
	if (c_pSplashWnd->Create(pParentWnd))
		c_pSplashWnd->UpdateWindow();
	else
	{
		delete c_pSplashWnd;
		c_pSplashWnd = NULL;
	}
}

bool SplashWnd::PreTranslateMessage(MSG* pMsg)
{
	if (c_pSplashWnd == NULL)
		return FALSE;

	// If we get a keyboard or mouse message, hide the splash screen.
	if (pMsg->message == WM_KEYDOWN ||
	    pMsg->message == WM_SYSKEYDOWN ||
	    pMsg->message == WM_LBUTTONDOWN ||
	    pMsg->message == WM_RBUTTONDOWN ||
	    pMsg->message == WM_MBUTTONDOWN ||
	    pMsg->message == WM_NCLBUTTONDOWN ||
	    pMsg->message == WM_NCRBUTTONDOWN ||
	    pMsg->message == WM_NCMBUTTONDOWN)
	{
		c_pSplashWnd->HideSplashScreen();
		return true;	// message handled here
	}

	return false;	// message not handled
}

bool SplashWnd::Create(Wnd* pParentWnd /*= NULL*/)
{
	bitmap = LoadBitmap(GetApp()->GetInstance(), MAKEINTRESOURCE(IDB_SPLASH));
	if (!bitmap)
		return false;

	BITMAP bm;
	::GetObject(bitmap, sizeof(BITMAP), &bm);

	return Wnd::Create(0, NULL, WS_POPUP | WS_VISIBLE, 0, 0, bm.bmWidth, bm.bmHeight, pParentWnd, NULL);
}

void SplashWnd::HideSplashScreen()
{
	DestroyWindow();
	GetApp()->GetMainWnd()->UpdateWindow();
}

bool SplashWnd::OnCreate(LPCREATESTRUCT lpcs)
{
	HWND hwnd = ::GetParent(*this);
	RECT rcCenter;
	if(hwnd)
		::GetWindowRect(hwnd, &rcCenter);
	else
	{
		rcCenter.left = 0;
		rcCenter.top = 0;
		rcCenter.right = GetSystemMetrics(SM_CXFULLSCREEN);
		rcCenter.bottom = GetSystemMetrics(SM_CYFULLSCREEN);
	}
	int x = (rcCenter.left + rcCenter.right) / 2 - lpcs->cx / 2;
	int y = (rcCenter.top + rcCenter.bottom) / 2 - lpcs->cy / 2;
	::SetWindowPos(*this, HWND_TOP, x, y, -1, -1, SWP_NOSIZE);
	return true;
}

void SplashWnd::OnPaint()
{
	PAINTSTRUCT paint;
	HDC hdc = ::BeginPaint(*this, &paint);
	HDC hdcImage;
	hdcImage = ::CreateCompatibleDC(hdc);
	if(!hdcImage)
		return;

	BITMAP bm;
	::GetObject(bitmap, sizeof(BITMAP), &bm);

	// Paint the image.
	HBITMAP hOldBitmap = ::SelectObject(hdcImage, bitmap);
	::BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcImage, 0, 0, SRCCOPY/*WHITENESS*/);
	::SelectObject(hdcImage, hOldBitmap);
	::DeleteDC(hdcImage);
	::EndPaint(*this, &paint);
}

void SplashWnd::OnTimer(UINT nIDEvent)
{
	// Destroy the splash screen window.
	HideSplashScreen();
}
