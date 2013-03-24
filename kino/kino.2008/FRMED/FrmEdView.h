// FrmEdView.h : interface of the CFrmEdView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FRMEDVIEW_H__F7961D10_0A02_11D2_A68D_204C4F4F5020__INCLUDED_)
#define AFX_FRMEDVIEW_H__F7961D10_0A02_11D2_A68D_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CField;
class CFrmEdDoc;
class CFrmEdView : public CScrollView
{
protected: // create from serialization only
	CFrmEdView();
	DECLARE_DYNCREATE(CFrmEdView)

// Attributes
public:
	CFrmEdDoc* GetDocument();
	int scale;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFrmEdView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

// Implementation
public:
	void Scale(int sc);
	void Load();
	void Save();
	virtual ~CFrmEdView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CFrmEdView)
	afx_msg void OnFieldNew();
	afx_msg void OnFieldFont();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in FrmEdView.cpp
inline CFrmEdDoc* CFrmEdView::GetDocument()
   { return (CFrmEdDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRMEDVIEW_H__F7961D10_0A02_11D2_A68D_204C4F4F5020__INCLUDED_)
