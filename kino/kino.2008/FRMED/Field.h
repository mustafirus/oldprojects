#if !defined(AFX_FIELD_H__FA5495F5_0A52_11D2_A68E_204C4F4F5020__INCLUDED_)
#define AFX_FIELD_H__FA5495F5_0A52_11D2_A68E_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Field.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CField window

class CField : public CEdit
{
// Construction
public:
	CField(const RECT& rect, CWnd* pParentWnd);

// Attributes
public:
	bool ro;
	CPoint lapt;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CField)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CField();

	// Generated message map functions
protected:
	//{{AFX_MSG(CField)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIELD_H__FA5495F5_0A52_11D2_A68E_204C4F4F5020__INCLUDED_)
