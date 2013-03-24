// Field.cpp : implementation file
//

#include "stdafx.h"
#include "frmed.h"
#include "FrmEdDoc.h"
#include "FrmEdView.h"
#include "Field.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CField

CField::CField(const RECT& rect, CWnd* pParentWnd) : ro(true)
{
	Create(ES_READONLY|WS_VISIBLE|WS_CHILD|ES_AUTOHSCROLL|WS_THICKFRAME, rect, pParentWnd, 0);
	if(((CFrmEdView*)pParentWnd)->GetDocument()->pFont)
		SetFont(((CFrmEdView*)pParentWnd)->GetDocument()->pFont);
	ModifyStyleEx(0, WS_EX_TRANSPARENT);
	SetFocus();
}

CField::~CField()
{
}


BEGIN_MESSAGE_MAP(CField, CEdit)
	//{{AFX_MSG_MAP(CField)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CField message handlers

void CField::OnLButtonDown(UINT nFlags, CPoint point) 
{
	lapt = point;
	BringWindowToTop();
	SetCapture();
	CEdit::OnLButtonDown(nFlags, point);
}

void CField::OnLButtonUp(UINT nFlags, CPoint point) 
{
	::ReleaseCapture();
	CEdit::OnLButtonUp(nFlags, point);
}

void CField::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if(ro)
	{
		SetReadOnly(FALSE);
		ro = false;
	}
	CEdit::OnLButtonDblClk(nFlags, point);
}

void CField::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar == VK_RETURN)
	{
		ro = !ro;
		SetReadOnly(ro);
	}else if(nChar == VK_DELETE && ro)
	{
		DestroyWindow();
	}else
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CField::OnKillFocus(CWnd* pNewWnd) 
{
	SetReadOnly();
	ro = true;
	CEdit::OnKillFocus(pNewWnd);
}

void CField::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(GetCapture() == this)
	{
		CRect rect;
		GetWindowRect(rect);
		GetParent()->ScreenToClient(rect);
		rect.OffsetRect(point-lapt);
		MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());
	}
	CEdit::OnMouseMove(nFlags, point);
}

LRESULT CField::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(message == WM_SETFONT)
	{
		int i = 1;
	}
	return CEdit::WindowProc(message, wParam, lParam);
}

LRESULT CField::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(message == WM_SETFONT)
	{
		int i = 1;
	}
	return CEdit::DefWindowProc(message, wParam, lParam);
}

void CField::PostNcDestroy() 
{
	delete this;
}
