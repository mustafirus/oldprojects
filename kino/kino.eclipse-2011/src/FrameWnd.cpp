// FrameWnd.cpp : implementation file
//

#include "stdx.h"
#include "dblib.h"
#include "kinores.h"
#include "WndLib.h"
#include "FrameThread.h"
#include "ErrorCodes.h"
#include "RFieldControl.h"
#include "Wait.h"
#include "MainWnd.h"

#include "defines.h"
#include "Styles.h"
#include "FDC.h"
#include "Action.h"
#include "Frame.h"
#include "Exception.h"
#include "FrameWnd.h"

//#include "Grid.h"
//#include "RFieldControl.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAXACCELS	10
#define BASECLASS MDIChild

// For new functionality
void FrameWnd::Action(Record* pr, RField* prf, int nID)
{
	Action::GetAction()->Exec(this, pr, prf, nID);
}

FrameWnd::FrameWnd(DataContext* pdc, FrameWnd* pPrev) :
next(NULL), prev(pPrev), hWndTracking(NULL), pRootFrame(NULL), /*pLastFocused(NULL),*/
curpos(TabMap::first_pos), hAccel(NULL), hMenu(NULL)
{
	Wait ww;
	if(prev)
		prev->next = this;
	if(next)
		next->prev = this;
	pdc->pWnd = this;
	bAutoSave = pdc->bAutoSave; 
	hMenu = pdc->hMenu;
	hAccel = pdc->hAccel;
	GetMainWnd()->CreateChildFrame(this, pdc->szFrameName, MDIS_ALLCHILDSTYLES, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, (LPARAM)pdc);
}

void FrameWnd::AdjustSize(SIZE& sz)
{
	POINT pt = {-defspace.left, -defspace.top};
	sz.cx += defspace.left + defspace.right;
	sz.cy += defspace.top + defspace.bottom;
	Rect r(pt,sz);
	AdjustWindowRect(r, GetStyle(), FALSE);
	sz = r;
	RECT mc;
	GetClientRect(*(GetMainWnd()), &mc);
	sz.cx = min(sz.cx, mc.right - 20);
	sz.cy = min(sz.cy, mc.bottom - 20);
}

FrameWnd::~FrameWnd()
{
	if(hMenu)
		DestroyMenu(hMenu);
	if(hAccel)
		DestroyAcceleratorTable(hAccel);
	if(prev)
		prev->next = next; 
	if(next)
		next->prev = prev;
	ifdel(pRec);
	ifdel(pRootFrame);
}

BEGIN_COMMAND_MAP(FrameWnd, BASECLASS)
	ON_COMMAND_RANGE(ID_RECORD_FIRST, ID_RECORD_LAST, OnCommands)
	ON_COMMAND_RANGE(ID_FORM_FIRST, ID_FORM_LAST, OnCommands)
	ON_COMMAND_RANGE(ID_PROC_FIRST, ID_PROC_LAST, OnCommands)
	ON_COMMAND_EX(ID_FILE_PRINT, OnCommands)
	ON_COMMAND_EX(ID_FILE_PRINT_SETUP, OnCommands)
//	ON_COMMAND_EX(ID_RECORD_INSERT, OnCommands)
//	ON_COMMAND_EX(ID_RECORD_DELETE, OnCommands)
//	ON_COMMAND_EX(ID_RECORD_DELETE, OnCommands)
END_COMMAND_MAP

LRESULT FrameWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	DISPATCH
		ON_MSG(WM_GETMINMAXINFO, OnGetMinMaxInfo);
		ON_MSG(WM_CREATE, OnCreate);
		ON_MSG(WM_MDIACTIVATE, OnMDIActivate);
		ON_MSG(WM_CLOSE, OnClose);
		ON_MSG(WM_WINDOWPOSCHANGED, OnWindowPosChanged);
		ON_MSG(WM_SIZE, OnSize);
		ON_MSG(WM_SETCURSOR, OnSetCursor)
		ON_MSG(WM_COPY, OnCopy)
		ON_MSG(WM_PASTE, OnPaste)
		ON_MSG(WM_SETFOCUS, OnSetFocus)
		ON_MSG(WM_PAINT, OnPaint);
		ON_MSG(WM_KEYDOWN, OnKey);
//		ON_MSG(WM_KEYUP, OnKey);
//		ON_MSG(WM_LBUTTONDOWN, OnLButtonDown);
//		ON_MSG(WM_LBUTTONDBLCLK, OnLButtonDown);
//		ON_MSG(WM_MOUSEMOVE, OnMouseMove);
//		ON_MSG(WM_ENABLE, OnEnable);
		ON_MSG(WM_DESTROY, OnDestroy);
//		ON_MSG(WM_NORM_REC, OnNorm);
	BASEROUGHT
}

bool FrameWnd::OnCommand(UINT nID, HWND hwndCtl, UINT codeNotify)
{
	Wnd* pWnd = GetFocus();
	Wnd* pWndParent;
	if(IsChild(pWnd))
	{
		while(pWnd && (pWndParent = pWnd->GetParent()) != this)
			pWnd = pWndParent;
		if(pWnd && pWnd->OnCommand(nID, hwndCtl, codeNotify))
			return true;
	}
	return Wnd::OnCommand(nID, hwndCtl, codeNotify);
}

void FrameWnd::OnSetFocus(HWND hwndOldFocus)
{
	Default();
		Wnd* pWnd =	tabmap.GetAt(curpos);
		if(pWnd)
			pWnd->SetFocus();
/*	if(pLastFocused)
		pLastFocused->SetFocus();
	else
		::SetFocus(GetWindow(*this, GW_CHILD));
*/
}

void FrameWnd::OnCopy()
{

}

void FrameWnd::OnPaste()
{

}

void FrameWnd::OnWindowPosChanged(const LPWINDOWPOS lpwpos)
{
	Default();
	bool nic;
	if(IsWindowVisible(*this))
	{
		if(nic = !IsIconic(*this))
		{
			ASSERT(pRootFrame);
			RECT r;
			GetClientRect(*this, &r);
			POINT pt = {defspace.left, defspace.top};
			SIZE& sz = *((SIZE*)((POINT*)&r)+1);//{r.right - defspace.left - defspace.right, lpwpos->cy - defspace.top - defspace.bottom};
			sz.cx = sz.cx  - defspace.left - defspace.right;
			sz.cy = sz.cy  - defspace.top - defspace.bottom;
			HDWP hdwp = BeginDeferWindowPos(10);
			pRootFrame->SetPos(pt, sz, hdwp);
			EndDeferWindowPos(hdwp);
		}
		ShowAll(nic);
	}
}

void FrameWnd::OnSize(UINT nType, int cx, int cy)
{
	Default();
}

bool FrameWnd::OnCreate(LPCREATESTRUCT lpcs)
{
	MDICREATESTRUCT* pmcs = (MDICREATESTRUCT*)lpcs->lpCreateParams;
	DataContext* pdc = (DataContext*)pmcs->lParam;
	ASSERT(pdc->szForm);
	char* str = pdc->szForm;
	ASSERT(str);
	try{
	pRootFrame = Frame::CreateFrame(str, pdc);
	}catch(Exception* e)
	{
		pdc->FreeContext();
		e->printf(MSG_ERROR_FRAME_SYNTAX_ERROR);
		e->Effect();
		return false;
	}
	if(!pRootFrame)
	{
		Exception::ex.printf(MSG_ERROR_FRAME_SYNTAX_ERROR);
		Exception::ex.Effect();
		return false;
	}
	pRec = pdc->GetRecord(false);//not create
	if(!pRec)
		pRecdef = pdc->GetDefRecord();//not create
	pdc->FreeContext();
	if(pRec && pRec->IsDummy())
	{
		if(!pRec->Load())
			return false;
	}else
	{
		_RPT0(_CRT_WARN, "On Create FrameWnd RecordSet alredy loaded !");
	}

	MINMAX mm;
	pRootFrame->GetMinMax(&mm);
	TabPos pos;

	_ASSERTE(_CrtCheckMemory());
	pRootFrame->SetTabs(&tabmap, TabMap::first_pos, pos);
	_ASSERTE(_CrtCheckMemory());
	
	POINT pt = {defspace.left, defspace.top};
	AdjustSize(mm.szDes);
	SetWindowPos(*this, NULL, 0,0, mm.szDes.cx, mm.szDes.cy, SWP_NOMOVE|
		SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOREDRAW|SWP_NOSENDCHANGING|
		SWP_NOOWNERZORDER);

//	SetCursorPos(lpcs->x + (lpcs->cx)/3, lpcs->y + (lpcs->cy)/2 );


	EnableWindow(*prev, FALSE);
//	SetFocus();
	LRESULT l = Default();
	return true;
}

void FrameWnd::OnGetMinMaxInfo(LPMINMAXINFO pmm)
{
	Default();
	if(pRootFrame)
	{
		MINMAX mm;
		POINT pt = {0,0};
		pRootFrame->GetMinMax(&mm);
		AdjustSize(mm.szMin);
		AdjustSize(mm.szMax);
//		pmm->ptMaxSize = *(POINT*)&mm.szMax;
		pmm->ptMinTrackSize = *(POINT*)&mm.szMin;
		pmm->ptMaxTrackSize = *(POINT*)&mm.szMax;
	}/*else
		Default();*/
}

bool FrameWnd::OnSetCursor(HWND hwndCursor, UINT codeHitTest, UINT msg)
{
	if(hwndCursor == *this)
	{
		Wnd* pWnd = hWndTracking ? FromHandle(hWndTracking) : NULL;
		if(pWnd)
			pWnd->HiliteWindow(false);
		hWndTracking = NULL;
	}else
	{
	HWND hChild = hwndCursor;
	while(hwndCursor && (hwndCursor = ::GetParent(hwndCursor)) != *this)
		hChild = hwndCursor;
	if(hChild && hChild != hWndTracking)
	{
		Wnd* pWnd = hWndTracking ? FromHandle(hWndTracking) : NULL;
		if(pWnd)
			pWnd->HiliteWindow(false);
		hWndTracking = hChild;
		pWnd = hWndTracking ? FromHandle(hWndTracking) : NULL;
		if(pWnd)
			pWnd->HiliteWindow(true);
	}
	}
	return Default() > 0;
}

void FrameWnd::OnMDIActivate(bool fActive, HWND hwndActivate, HWND hwndDeactivate)
{
	Default();
	if(fActive)
	{
		Wnd* pWnd =	tabmap.GetAt(curpos);
		if(pWnd)
			pWnd->SetFocus();
	}

	if(		( fActive && (hwndDeactivate == *next || hwndDeactivate == *prev)) ||
		(!fActive && (hwndActivate == *next || hwndActivate == *prev))		)
		return;
	ShowAll(fActive);
	HMENU hMenu = GetMainWnd()->RefreshMenu();
}

void FrameWnd::ShowAll(bool sa_show)
{
	FrameWnd* pShow = prev;
	while(pShow)
	{
		pShow->ShowWindow((sa_show ? SW_SHOW : SW_HIDE));
		pShow = pShow->prev;
	}
}

void FrameWnd::OnClose() 
{
	Wait w;
	if(pRec && pRec->IsModified())
	{
		if(bAutoSave)
			pRec->Save();
		else
		{
			switch(MessageBox(*this, "Сохранить изменения?", "Kino", MB_YESNOCANCEL|MB_ICONQUESTION))
			{
			case IDYES:
				pRec->Save();
				break;
			case IDCANCEL:
				return;
			}
		}
	}
	DestroyWindow();
	return;
}

bool FrameWnd::DestroyWindow()
{
	if(GetThreadWnd() == this)
		PostQuitMessage(0);
	if(GetApp() == GetApp()->GetCurrent())
	{
		::SendMessage(::GetParent(*this), WM_MDIDESTROY, (WPARAM)(HWND)(*this), 0);
	}else
	{
		::SendMessage(::GetParent(*this), WM_MDIDESTROY, (WPARAM)(HWND)(*this), 0);
		return ::DestroyWindow(*this) > 0;
	}
	return true;
};

void FrameWnd::OnPaint() 
{
	PAINTSTRUCT paint;
	HDC hdc = BeginPaint(*this, &paint);
//	FDC dc(this);
	SetBkMode(hdc, TRANSPARENT);
	pRootFrame->Draw(hdc);
}

void FrameWnd::OnKey(UINT nChar, bool fDown, int nRepCnt, UINT nFlags)
{
//	Frame::FocusDir dir=Frame::none;
//	if(!fDown)
	TabDir dir ={0,0};
	switch(nChar)
	{
		case VK_HOME: break;
//		case VK_END: dir = TabMap::end; break;
//		case VK_PRIOR: dir = TabMap::pageup; break;
//		case VK_NEXT: dir = TabMap::pagedown; break;
		case VK_UP: dir = TabMap::up; break;
		case VK_DOWN: dir = TabMap::down; break;
		case VK_LEFT: dir = TabMap::left; break;
		case VK_RIGHT: dir = TabMap::right; break;
//		case VK_RETURN:
		case VK_ESCAPE:
			OnClose();
			return;
	}
	if(dir.r != 0 || dir.c != 0)
	{
		Wnd* pWnd =	tabmap.GetNext(curpos, dir);
		if(pWnd)
			pWnd->SetFocus();
		return;
	}else

	Default();
}

void FrameWnd::OnLButtonDown(bool fDoubleClick, int x, int y, UINT keyFlags)
{
	if(!fDoubleClick)
	if(keyFlags == MK_LBUTTON)
	{
		SetFocus();
	}
	Default();
}


void FrameWnd::OnDestroy()
{
	if(next)
		next->DestroyWindow();
	EnableWindow(*prev, TRUE);
	GetMainWnd()->MDIActivate(prev);
}

void FrameWnd::OnMouseMove(int x, int y, UINT keyFlags)
{
/*	POINT pt = {x, y};
	HWND hwnd = ::ChildWindowFromPoint(*this, pt);
	if(hwnd == hWndTracking)
		return;
	if(hWnd == hWndTracking)
		return;
	if(hWndTracking)
		::SendMessage(hWndTracking, WM_MOUSEENTER, (WPARAM)FALSE, 0);
	hWndTracking = hWnd;
	if(hWndTracking)
		::SendMessage(hWndTracking, WM_MOUSEENTER, (WPARAM)TRUE, 0);
*/
}

char* FrameWnd::GetControls(char* str, RFieldControls& vct)
{
	ASSERT(pRec);
	RFields rf;
	str = pRec->GetRFields(str, rf);
	for(int i = 0; i < rf.GetCount(); i++)
	{
		vct.Add(vCtrls.Add(new RFieldControl(rf[i], this, vCtrls.GetCount())));
	}
	return str;
}

void FrameWnd::OnCommands(UINT nID)
{
	Wait w;
	switch(nID)
	{
	case ID_RECORD_SAVE:
		if(pRec)
			pRec->Save();
		InvalidateRect(NULL);
		break;
	case ID_RECORD_REFRESH:
		if(pRec)
			pRec->Load();
		InvalidateRect(NULL);
		return;
	case ID_RECORD_UNDO:
		if(pRec)
			pRec->Undo();
		InvalidateRect(NULL);
		return;
	};
	Action::GetAction()->Exec(this, (pRec ? pRec : pRecdef), NULL, nID);
};

MainWnd* FrameWnd::GetMainWnd()
{
	return ((MainWnd*)Application::GetApp()->GetMainWnd());
}

/*FrameWnd::FrameWnd(const char* name, const char* str, Record* pr) :pRec(pr)
{
	GetMainWnd()->CreateChildFrame(this, name, WS_SYSMENU|
		WS_CAPTION|WS_MINIMIZEBOX|WS_CHILD, 0, 0, 0, 0);
//	Wnd* pOldWnd = Frame::pWnd;	Frame::pWnd = this;
	pRootFrame = Frame::CreateFrame((char*&)str, this);
	pRootFrame->CalcSizes();
	pRootFrame->SetSize(pRootFrame->GetRSize());
	pRootFrame->SetSizes();
	pRootFrame->SetPos(defspace.left,300+defspace.top);
	SIZE sz;
	pRootFrame->GetSize(sz);
	sz.cx += defspace.left + defspace.right + 100;
	sz.cy += defspace.top + defspace.bottom + 100;
	Rect r(Point(0,0),sz);
//	CalcWindowRect(*this, r);
	AdjustWindowRect(r, GetStyle(), FALSE);
	SetWindowPos(*this, NULL, 0,0, r.Width(), r.Height(), SWP_NOMOVE|SWP_NOZORDER);
	GetWindowRect(*this, r);
	SetCursorPos(r.left + (r.right - r.left)/3, r.top + (r.bottom - r.top)/2 );
	pRootFrame->MoveFocus(Frame::up);
	pRec->Load();
	ShowWindow(SW_SHOW);
}
*/


/*bool FrameWnd::OnCmdUI(CmdUI* pCmdUI)
{
	bool ret = false;
	switch(nID)
	{
	case ID_FILE_SAVE:
	case ID_FILE_EXPORT:
	case ID_RECORD_DELETE:
	case ID_RECORD_INSERT:
		ret = true;
		break;
	default:
		ret = pAction->CanUserAction(nID);
	};
//	ret = true;
	if(ret)
		pCmdUI->Enable();
	return ret;
}
*/
/////////////////////////////////////////////////////////////////////////////
// FrameWnd

//FrameWnd::FrameWnd(/*ScreenAction* pa, char* str, CreateContext* pContext*/) /*:Wnd(pa)*/
/*{
	Wnd* pOldWnd = Frame::pWnd;	Frame::pWnd = this;
	Create(AfxRegisterWndClass( CS_DBLCLKS,LoadCursor(NULL,IDC_ARROW), 
		CreateSolidBrush(RGB(255,255,240))),
		NULL, WS_SYSMENU|WS_CAPTION|WS_MINIMIZEBOX|WS_CHILD|WS_CLIPCHILDREN);
	ModifyStyle(WS_BORDER|WS_DLGFRAME, WS_MINIMIZEBOX|WS_CAPTION);
	ModifyStyleEx(WS_EX_CLIENTEDGE | WS_EX_WINDOWEDGE, 0);
	Init(str, pContext);
	Frame::pWnd = pOldWnd;
}*/

/////////////////////////////////////////////////////////////////////////////
// FrameWnd message handlers

/*BOOL FrameWnd::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	CWnd* pWnd = GetFocus();
	if(IsChild(pWnd))
		return pWnd->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	if(Wnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;
	else
		return CMDIChildWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
*/
FrameWnd* FrameWnd::GetThreadWnd()
{
	return ((FrameThread*)Thread::GetCurrent())->GetThreadWnd();
}

RECT FrameWnd::defspace = {5,5,5,5};

/*Wnd::Wnd(ScreenAction* pa) : pAction(pa), modified(false),
pRootFrame(NULL), lRes(0), bEnabled(false), bStarted(false)
{
	m_hAccelTable = LoadAccelerators(NULL, MAKEINTRESOURCE(IDR_FRAME));
};
*/
void FrameWnd::RegisterClass()
{
	Wnd::RegisterClass(CS_OWNDC|CS_DBLCLKS, 0, LoadCursor(NULL,IDC_ARROW),
		CreateSolidBrush(RGB(255,255,240)), 0, "FRAMEWND", 0);
}

void FrameWnd::BringToTop(bool top)
{
	HWND hwnd; 
	if(top)
		hwnd = HWND_TOP;
	else
	{
		hwnd = *next;
		ASSERT(hwnd);
	}
	SetWindowPos(*this, hwnd, -1,-1,-1,-1, 
		SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOSENDCHANGING);
}

/*void FrameWnd::Init(char* str, CreateContext* pContext)
{
	pContext->pWnd = this;
	FDC fdc(this, new CClientDC(GetWnd()));
	pContext->pDC = &fdc;
	pRootFrame = pContext->pFrame ? pContext->pFrame : Frame::CreateFrame(str, pContext);
	pRootFrame->CalcSizes();
	pRootFrame->SetSize(pRootFrame->GetRSize());
	pRootFrame->SetSizes();
	pRootFrame->SetPos(CPoint(defspace.left,defspace.top));
	SIZE sz;
	pRootFrame->GetSize(sz);
	sz.cx += defspace.left + defspace.right;
	sz.cy += defspace.top + defspace.bottom;
	CRect r(CPoint(0,0),sz);
	GetWnd()->CalcWindowRect(r);
	GetWnd()->SetWindowPos(NULL, 0,0, r.Width(), r.Height(), SWP_NOMOVE|SWP_NOZORDER);
	GetWnd()->GetWindowRect(r);
	SetCursorPos(r.left + (r.right - r.left)/3, r.top + (r.bottom - r.top)/2 );
	pRootFrame->MoveFocus(Frame::up);
	bEnabled = true;
}

*/
bool FrameWnd::OnCmdUI(CmdUI* pCmdUI)
{
	Wnd* pWnd = GetFocus();
	Wnd* pWndParent;
	if(IsChild(pWnd))
	{
		while(pWnd && (pWndParent = pWnd->GetParent()) != this)
			pWnd = pWndParent;
	}
		
	if(pWnd && pWnd->OnCmdUI(pCmdUI))
		return true;
/*	UINT nID = *pCmdUI;
	if(nID >= ID_EDIT_FIRST && nID <= ID_EDIT_LAST)
	{
		pCmdUI->Enable();
		return true;
	}
*/	return MDIChild::OnCmdUI(pCmdUI);
}

void FrameWnd::UpdateMenu(HMENU hmenu)
{
	MENUITEMINFO mi;
	memset(&mi, 0, sizeof(mi));
	mi.cbSize = sizeof(mi);
	if(hMenu)
	{
		mi.fMask = MIIM_STATE|MIIM_SUBMENU;
		mi.fState = MFS_ENABLED;
		mi.hSubMenu = hMenu;
	}else
	{
		mi.fMask = MIIM_STATE;
		mi.fState = MFS_DISABLED;
	}
	SetMenuItemInfo(hmenu, GetMenuItemCount(hmenu)-1, TRUE, &mi);
}

bool FrameWnd::PreTranslateMessage(MSG* pMsg)
{
	return (hAccel ? (::TranslateAccelerator(*this, hAccel, pMsg)>0) : false);
}

/*static void CrackAcc(char* acstr, UINT wID, ACCEL* pac, int cac);
void FrameWnd::Fun(QTable * pqt, UINT nID)
{
//	ASSERT(nID > 0);
//	nID--;

	NAMEINFO ni;
	MENUITEMINFO mi;
	memset(&mi, 0, sizeof(mi));
	mi.cbSize = sizeof(mi);
	pqt->GetNames(&ni);
//	((char*)szFrameName) = ni.names[nID];
	hAccel = GetApp()->LoadAccelerators(((Table*)*pqt)->name);
	ACCEL ac[MAXACCELS];
	int cac = CopyAcceleratorTable(hAccel, ac, MAXACCELS);

	char str[50];
	if(ni.fnum>2 || ni.pnum>0)
		hMenu = CreateMenu();
	mi.fMask = MIIM_TYPE;
	mi.fType = MFT_STRING|MIIM_ID;
	while(ni.pnum)
	{
		mi.wID = ID_PROC_FIRST + ni.fnum;
		ni.pnum--;
		strcpy(str, ni.names[ni.fnum + ni.pnum]);
		CrackAcc(str + strlen(str), mi.wID, ac, cac);
		mi.dwTypeData = str;
		if(mi.dwTypeData)
			InsertMenuItem(hMenu, 0, TRUE, &mi);
	}
	if(ni.fnum>2 && ni.pnum>0)
	{
		mi.fType = MFT_SEPARATOR;
		InsertMenuItem(hMenu, 0, TRUE, &mi);
	}
	mi.fType = MFT_STRING|MIIM_ID;
	while(ni.fnum>2)
	{
		mi.wID = ID_FORM_FIRST + ni.fnum;
		ni.fnum--;
		strcpy(str, ni.names[ni.fnum]);
		CrackAcc(str + strlen(str), mi.wID, ac, cac);
		mi.dwTypeData = str;
		if(mi.dwTypeData)
			InsertMenuItem(hMenu, 0, TRUE, &mi);
	}
	delete ni.names;
}

static void CrackAcc(char* acstr, UINT wID, ACCEL* pac, int cac)
{
	while(cac)
	{
		cac--;
		if(pac->cmd == wID)
		{
			const char* szalt = (pac->fVirt&FALT) ? "Alt+" : "";
			const char* szctrl = (pac->fVirt&FCONTROL) ? "Ctrl+" : "";
			const char* szshift = (pac->fVirt&FSHIFT) ? "Shift+" : "";
			sprintf(acstr, "\t%s%s%s%c", szalt, szctrl, szshift, pac->key);
		}
	}
}
*/