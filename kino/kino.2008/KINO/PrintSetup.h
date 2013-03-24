#if !defined(AFX_PRINTSETUP_H__5F42A742_20F7_11D2_A6C8_204C4F4F5020__INCLUDED_)
#define AFX_PRINTSETUP_H__5F42A742_20F7_11D2_A6C8_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PrintSetup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PrintSetup dialog

void GetPrintOffs(int type, int& left, int& top);
class PrintSetup : public CDialog
{
// Construction
public:
	PrintSetup(CWnd* pParent = NULL);   // standard constructor
	~PrintSetup();

// Dialog Data
	//{{AFX_DATA(PrintSetup)
	enum { IDD = IDD_PRINT_PROP };
	int		m_type;
	UINT	m_loff;
	UINT	m_toff;
	//}}AFX_DATA
	UINT	offs[6];


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PrintSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(PrintSetup)
	afx_msg void OnDefoff();
	afx_msg void OnSetdefoff();
	afx_msg void OnSelchangeDoctype();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRINTSETUP_H__5F42A742_20F7_11D2_A6C8_204C4F4F5020__INCLUDED_)
