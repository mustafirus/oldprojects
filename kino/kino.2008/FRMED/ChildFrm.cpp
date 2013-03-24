// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "FrmEd.h"
//#include "FrmEdDoc.h"
#include "FrmEdView.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	ON_WM_CREATE()
	ON_COMMAND_EX(CG_ID_VIEW_MYDIALOGBAR, OnBarCheck)
	ON_UPDATE_COMMAND_UI(CG_ID_VIEW_MYDIALOGBAR, OnUpdateControlBarMenu)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	ON_CONTROL(EN_KILLFOCUS, IDC_SCALE, OnShangeScale)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
	
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CMDIChildWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add a menu item that will toggle the visibility of the
	// dialog bar named "My Dialog Bar":
	//   1. In ResourceView, open the menu resource that is used by
	//      the CChildFrame class
	//   2. Select the View submenu
	//   3. Double-click on the blank item at the bottom of the submenu
	//   4. Assign the new item an ID: CG_ID_VIEW_MYDIALOGBAR
	//   5. Assign the item a Caption: My Dialog Bar

	// TODO: Change the value of CG_ID_VIEW_MYDIALOGBAR to an appropriate value:
	//   1. Open the file resource.h
	// CG: The following block was inserted by the 'Dialog Bar' component
	{
		// Initialize dialog bar m_wndMyDialogBar
		if (!m_wndMyDialogBar.Create(this, CG_IDD_MYDIALOGBAR,
			CBRS_BOTTOM | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,
			CG_ID_VIEW_MYDIALOGBAR))
		{
			TRACE0("Failed to create dialog bar m_wndMyDialogBar\n");
			return -1;		// fail to create
		}

		m_wndMyDialogBar.EnableDocking(CBRS_ALIGN_BOTTOM | CBRS_ALIGN_TOP);
		EnableDocking(CBRS_ALIGN_ANY);
		DockControlBar(&m_wndMyDialogBar);
		m_wndMyDialogBar.SetDlgItemText(IDC_SCALE, "100%");
	}

	return 0;
}
void CChildFrame::OnShangeScale()
{
	CString str;
	m_wndMyDialogBar.GetDlgItemText(IDC_SCALE, str);
	int scale = atoi(str);
	str.Format("%d%%", scale);
	m_wndMyDialogBar.SetDlgItemText(IDC_SCALE, str);
	CFrmEdView* pv = (CFrmEdView*)GetActiveView();
	if(pv)
		pv->Scale(scale);
}

void CChildFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CMDIChildWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CChildFrame::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		SetFocus();
	return CMDIChildWnd::PreTranslateMessage(pMsg);
}
