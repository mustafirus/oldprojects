// FrmEdView.cpp : implementation of the CFrmEdView class
//

#include "stdafx.h"
#include "FrmEd.h"
#include "Field.h"

#include "FrmEdDoc.h"
#include "FrmEdView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFrmEdView

IMPLEMENT_DYNCREATE(CFrmEdView, CView)

BEGIN_MESSAGE_MAP(CFrmEdView, CView)
	//{{AFX_MSG_MAP(CFrmEdView)
	ON_COMMAND(ID_FIELD_NEW, OnFieldNew)
	ON_COMMAND(ID_FIELD_FONT, OnFieldFont)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFrmEdView construction/destruction

CFrmEdView::CFrmEdView() :scale(100)
{
	// TODO: add construction code here

}

CFrmEdView::~CFrmEdView()
{
}

BOOL CFrmEdView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CFrmEdView drawing

void CFrmEdView::OnDraw(CDC* pDC)
{
	CFrmEdDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDC->SaveDC();
	pDC->SetMapMode(MM_HIMETRIC);
	pDC->SetWindowOrg(0,0);
	pDC->SetViewportOrg(0,0);
	pDoc->Bk.DrawBitmapHiMetric(pDC, scale);
	pDC->RestoreDC(-1);
}

/////////////////////////////////////////////////////////////////////////////
// CFrmEdView diagnostics

#ifdef _DEBUG
void CFrmEdView::AssertValid() const
{
	CView::AssertValid();
}

void CFrmEdView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFrmEdDoc* CFrmEdView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFrmEdDoc)));
	return (CFrmEdDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFrmEdView message handlers

void CFrmEdView::OnFieldNew() 
{
	new CField(CRect(20,20,200,40), this);
}

void CFrmEdView::OnFieldFont() 
{
	CFont*& pFont = GetDocument()->pFont;
	LOGFONT lf;
	memset(&lf,0,sizeof(lf));
	if(pFont)
		pFont->GetLogFont(&lf);
	CFontDialog dlg(&lf);
	dlg.DoModal();
	dlg.GetCurrentFont(&lf);
	if(pFont)
		pFont->DeleteObject();
	else
		pFont = new CFont;
	pFont->CreateFontIndirect(&lf);
	SendMessageToDescendants(WM_SETFONT, (WPARAM)pFont->GetSafeHandle(), TRUE);
}

void CFrmEdView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	SetScrollSizes(MM_HIMETRIC, CSize(10000,10000));
	Load();
}

void CFrmEdView::Save()
{
	Scale(100);
	CRectArray& frect = GetDocument()->frect;
	CStrArray& ftext = GetDocument()->ftext;
	frect.RemoveAll();
	ftext.RemoveAll();
	CWnd* pWnd = GetWindow(GW_CHILD);
	int i=0;
	CClientDC dc(this);
	dc.SaveDC();
	dc.SetMapMode(MM_HIMETRIC);
	dc.SetWindowOrg(0,0);
	dc.SetViewportOrg(0,0);
	while(pWnd)
	{
		CRect* pr = new CRect;
		frect.Add(pr);
		ftext.Add(new CString);
		pWnd->GetWindowRect(frect[i]);
		ScreenToClient(frect[i]);
		dc.DPtoLP(frect[i]);
		pWnd->GetWindowText(*ftext[i++]);
		pWnd = pWnd->GetNextWindow();
	}
	dc.RestoreDC(-1);
}


void CFrmEdView::Load()
{
	CRectArray& frect = GetDocument()->frect;
	CStrArray& ftext = GetDocument()->ftext;
	int n = frect.GetSize();
	CClientDC dc(this);
	dc.SaveDC();
	dc.SetMapMode(MM_HIMETRIC);
	dc.SetWindowOrg(0,0);
	dc.SetViewportOrg(0,0);
	for(int i = 0; i < n; i++)
	{
		dc.LPtoDP(frect[i]);
		CField* pf = new CField(*frect[i], this);
		pf->SetWindowText(*ftext[i]);
	}
	dc.RestoreDC(-1);
}

void CFrmEdView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar == VK_RETURN)
		GetNextDlgTabItem(GetFocus())->SetFocus();
	else
		CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
void CFrmEdView::Scale(int sc)
{
	if(sc == scale)
		return;
	CWnd* pWnd = GetWindow(GW_CHILD);
	while(pWnd)
	{
		CRect rect;
		pWnd->GetWindowRect(rect);
		ScreenToClient(rect);
		rect.top = rect.top * sc / scale;
		rect.left = rect.left * sc / scale;
		rect.bottom = rect.bottom * sc / scale;
		rect.right = rect.right * sc / scale;
		pWnd->MoveWindow(rect);
		pWnd = pWnd->GetNextWindow();
	}
	scale = sc;
	Invalidate();
}
