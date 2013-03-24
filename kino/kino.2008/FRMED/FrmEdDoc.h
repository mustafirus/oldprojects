// FrmEdDoc.h : interface of the CFrmEdDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FRMEDDOC_H__F7961D0E_0A02_11D2_A68D_204C4F4F5020__INCLUDED_)
#define AFX_FRMEDDOC_H__F7961D0E_0A02_11D2_A68D_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "dib.h"
#include "afxtempl.h"

typedef CTypedPtrArray <CPtrArray, CRect*>CRectArray;
typedef CTypedPtrArray <CPtrArray, CString*>CStrArray;

class CFrmEdDoc : public CDocument
{
protected: // create from serialization only
	CFrmEdDoc();
	DECLARE_DYNCREATE(CFrmEdDoc)

// Attributes
public:
	CDIBitmap Bk;
	CFont* pFont;
	CRectArray	frect;
	CStrArray	ftext;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFrmEdDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFrmEdDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CFrmEdDoc)
	afx_msg void OnFileChangeBmp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRMEDDOC_H__F7961D0E_0A02_11D2_A68D_204C4F4F5020__INCLUDED_)
