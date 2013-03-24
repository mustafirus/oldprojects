// RFieldControl.cpp: implementation of the RFieldControl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdx.h"
#include "dblib.h"
#include "resource.h"
#include "Log.h"
#include "WinThread.h"
#include "Kinores.h"
#include "Action.h"
#include "Styles.h"
#include "Wait.h"
#include "RFieldControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define BASECLASS EditControl
/*RFieldControl::RFieldControl(RField* prf, RECT* pr, Wnd* pWndParent, UINT nID):
Edit(0, WS_VISIBLE|WS_CHILD, pr, pWndParent, nID), pRField(prf)
{
	SetWindowText(*prf);
}
*/
RFieldControl::RFieldControl(RField* prf, Wnd* pWndParent, UINT nID):
EditControl(pWndParent,0, nID), /*Caption(0, 0, 0,0,0,0,  pWndParent),*/
pRField(prf), style(0), ver(0)
{
	FStyle Style;
	::GetStyle(GetFieldStyle(0, 0), &Style);
	SetFont(*Style.pFont);
	DefWindowProc(EM_LIMITTEXT, pRField->GetLen()-1, 0);
}

const char* RFieldControl::GetCaption()
{
	return Record::pDict->GetCapt(*pRField);
}

void RFieldControl::GetMinMax(PMINMAX pmm)
{
	if(*(UINT*)&minmax == 0)
	{
		TEXTMETRIC* ptm = GetTextMetrics(style);
		minmax.szMin.cx = ptm->tmAveCharWidth * ((Field*)*pRField)->scrlen * 3 / 2  + border.left + border.right;
		minmax.szMax.cx = ptm->tmAveCharWidth * ((Field*)*pRField)->strlen * 3 / 2  + border.left + border.right;
		minmax.szMin.cy = 
			minmax.szMax.cy = ptm->tmHeight + border.top + border.bottom;
	}
	*pmm = minmax;
}

/*void RFieldControl::GetDims(RDIMS* pdims)
{
	TEXTMETRIC* ptm = GetTextMetrics(style);
	pdims->capt.cx = ptm->tmAveCharWidth * strlen(GetCaption()) * 3 / 2;
	pdims->capt.cy = ptm->tmHeight;
	pdims->data.cx = ptm->tmAveCharWidth * ((Field*)*pRField)->scrlen * 3 / 2;
	pdims->data.cy = ptm->tmHeight;
}
*/

void RFieldControl::GetCaptionSize(SIZE& sz)
{
	TEXTMETRIC* ptm = GetTextMetrics(style);
	sz.cx = ptm->tmAveCharWidth * strlen(GetCaption()) * 3 / 2;
	sz.cy = ptm->tmHeight;
}

RFieldControl::~RFieldControl()
{
	ASSERT(pRField);
}

HBRUSH RFieldControl::OnCtlColorReflect(HDC hdc, int type)
{
	FStyle Style;
	::GetStyle(0, &Style);
	SetTextColor(hdc, Style.Fcolor);
	return BASECLASS::OnCtlColorReflect(hdc, type);
}
////////////////////////////////////////////////////////////
// Message Handlers
////////////////////////////////////////////////////////////
LRESULT RFieldControl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	DISPATCH
		ON_MSG(WM_KILLFOCUS, OnKillFocus)
		ON_MSG(WM_KEYDOWN, OnKey)
		ON_MSG(WM_LBUTTONDBLCLK, OnLButtonDblClk)
		ON_MSG(WM_MENUSELECT, OnMenuSelect)
		ON_MSG(WM_SETCURSOR, OnSetCursor)
		ON_MSG(WM_CONTEXTMENU, OnContextMenu)
		ON_MSG(WM_PAINT, OnPaint)
	BASEROUGHT
}

void RFieldControl::OnPaint()
{
	if(pRField->ver() != ver)
	{
		SetWindowText(*pRField);
		ver = pRField->ver();
	}
	Default();
}

void RFieldControl::OnContextMenu(HWND hwndContext, UINT xPos, UINT yPos)
{
	HMENU hm = GetApp()->LoadMenu(IDR_EDIT);
	HMENU sm = GetSubMenu(hm, 0);
//	SendMessage(WM_INITMENUPOPUP, (WPARAM)sm, 0);
	UINT n = TrackPopupMenu(sm, TPM_LEFTALIGN|TPM_TOPALIGN|
		TPM_LEFTBUTTON,	xPos, yPos, 0, *this, NULL);
}

bool RFieldControl::OnCmdUI(CmdUI* pCmdUI)
{
	bool en = false;
	UINT nID = *pCmdUI;
	if(nID >= ID_EDIT_FIRST && nID <= ID_EDIT_LAST)
	{
		if(pRField->ro())
			switch(nID)
		{
			case ID_EDIT_PASTE:
				{
					Wait w;
					if(pRField->CanPaste(*this))
						return pCmdUI->Enable(), true;
					return false;
				}
			case ID_EDIT_COPY:
				{
					Wait w;
					if(pRField->CanCopy())
						return pCmdUI->Enable(), true;
					return false;
				}
			case ID_EDIT_CLEAR:
				return pCmdUI->Enable(), true;
				return false;
		}
		else	
			switch(nID)
		{
		case ID_EDIT_CUT:
		case ID_EDIT_COPY:
			{
				UINT n = DefWindowProc(EM_GETSEL);
				if(LOWORD(n) != HIWORD(n)  || !edit)
					return pCmdUI->Enable(), true;
			}
			break;
		case ID_EDIT_PASTE:
			if(OpenClipboard(*this) &&
				IsClipboardFormatAvailable(CF_TEXT) && CloseClipboard())
				return pCmdUI->Enable(), true;
			break;
		case ID_EDIT_CLEAR:
		case ID_EDIT_SELECT_ALL:
			return pCmdUI->Enable(), true;
			break;
		case ID_EDIT_UNDO:
			if(DefWindowProc(EM_CANUNDO))
				return pCmdUI->Enable(), true;
			break;
		}
		return false;
	}
	return EditControl::OnCmdUI(pCmdUI);
}


void RFieldControl::OnMenuSelect(HMENU hmenu, int item, HMENU hmenuPopup, UINT flags)
{
	ResendMessage(*(GetApp()->GetMainWnd()));
	item = item ? 0 : (item - ID_EDIT_FIRST + WM_COPY);
	DefWindowProc(WM_MENUSELECT, MAKELONG(item, flags), (LPARAM)hmenuPopup);
}

bool RFieldControl::OnSetCursor(HWND hwndCursor, UINT codeHitTest, UINT msg)
{
	if(pRField->ro())
	{
		SetCursor(GetApp()->LoadCursor(IDC_HAND));
		return false;
	}
	return Default() > 0;
}

void RFieldControl::OnLButtonDblClk(bool fDoubleClick, int x, int y, UINT keyFlags)
{
	if(pRField->ro())
	{
		if(MK_CONTROL & keyFlags)
			Activate(VK_CONTROL);
		else if(MK_SHIFT & keyFlags)
			Activate(VK_SHIFT);
		else
			Activate(0);
		return;
	}
	if(!edit)
		SetEditMode(true);
	else
		Default();
}

void RFieldControl::OnKillFocus(HWND hwndNewFocus)
{
	if(edit)
	{
		if(IsModified())
		{
			GetWindowText(pRField->GetBuf(), pRField->GetLen());
			pRField->SetModified();
			ver = pRField->ver();
		}
		SetEditMode(false);
	}
	EditControl::OnKillFocus(hwndNewFocus);
}


void RFieldControl::OnKey(UINT vk, bool fDown, int cRepeat, UINT flags)
{
	Wait w;
	if(fDown)
	switch(vk)
	{
	case VK_DELETE:
		if(!edit)
		{
			pRField->Delete();
			Invalidate();
			return;
		}
		break;
	case VK_ESCAPE:
		if(edit)
		{
			SetWindowText(*pRField);
			SetEditMode(false);
			return;
		}
		break;
	case VK_RETURN:
		if(edit)
		{
			GetWindowText(pRField->GetBuf(), pRField->GetLen());
			pRField->SetModified();
			ver = pRField->ver();
		}else if(pRField->ro())
		{
			if(GetKeyState(VK_CONTROL)&0x8000)
				Activate(VK_CONTROL);
			else if(GetKeyState(VK_SHIFT)&0x8000)
				Activate(VK_SHIFT);
			else
				Activate(0);
			return;
		}
		SetEditMode(!edit);
		return;
	}
	if(edit)
		BASECLASS::OnKey(vk, fDown, cRepeat, flags);
	else
		ResendMessage(::GetParent(*this));
}

void RFieldControl::OnChar(TCHAR ch, int cRepeat)
{
	switch(ch)
	{
	case VK_RETURN:
	case VK_ESCAPE:
		return;
	}
	if(pRField->ro())
		return;
	if(!edit)
	{
		MSG msg = GetLastMsg();
		SetWindowText(NULL);
		SetEditMode(true);
		DefWindowProc(msg.message, msg.wParam, msg.lParam);
	}
	EditControl::OnChar(ch, cRepeat);
}

void RFieldControl::Activate(UINT syskey)
{
	UINT id = ID_FORM_DEFAULT;
	switch(syskey)
	{
	case VK_CONTROL:
		id |= ACT_FLAG_THREAD;
		break;
/*	case VK_SHIFT:
		id = -2;
*/	}
	Action::GetAction()->Exec(((FrameWnd*)GetParent()), NULL, pRField, id);
}

BEGIN_COMMAND_MAP(RFieldControl, BASECLASS)
	ON_COMMAND_RANGE(ID_EDIT_FIRST, ID_EDIT_LAST, OnEdit)
END_COMMAND_MAP

void RFieldControl::OnEdit(UINT nID)
{
	Wait w;
	if(GetFocus() != this)
		SetFocus();
	if(pRField->ro())
		switch(nID)
	{
			case ID_EDIT_PASTE:
				pRField->OnPaste(*this);
				SetWindowText(*pRField);
				return;
			case ID_EDIT_COPY:
				pRField->OnCopy(*this);
				return;
			case ID_EDIT_CLEAR:
				RKey* prk = *pRField;
				prk->SetNull();
				((Record*)*pRField)->Refresh(prk);
				SetWindowText(*pRField);
				return;
	}
	else
		switch(nID)
	{
	case ID_EDIT_COPY:
		if(!edit)
			DefWindowProc(EM_SETSEL, 0, -1);
		DefWindowProc(nID - ID_EDIT_FIRST + WM_CUT);
		if(!edit)
			DefWindowProc(EM_SETSEL);
		Invalidate();
		return;
	case ID_EDIT_CUT:
		if(edit)
		{
			UINT n = DefWindowProc(EM_GETSEL);
			if(LOWORD(n) == HIWORD(n))
				return;
		}
	case ID_EDIT_PASTE:
		{
			BOOL clip = false;
			if(nID == ID_EDIT_PASTE)
			{
				if(OpenClipboard(*this))
				{
					clip = IsClipboardFormatAvailable(CF_TEXT);
					CloseClipboard();
				}
				if(!clip)
					return;
			}
		}
		if(!edit)
		{
			SetEditMode(true);
			DefWindowProc(EM_SETSEL, 0, -1);
		}
	case ID_EDIT_UNDO:
		if(nID == ID_EDIT_UNDO && !DefWindowProc(EM_CANUNDO))
			return;
	case ID_EDIT_CLEAR:
		if(!edit)
			SetEditMode(true);
		DefWindowProc(nID - ID_EDIT_FIRST + WM_CUT);
		return;
	case ID_EDIT_SELECT_ALL:
		if(!edit)
			SetEditMode(true);
		DefWindowProc(EM_SETSEL, 0, -1);
		return;
	default:
		ASSERT(FALSE);
	}
}
