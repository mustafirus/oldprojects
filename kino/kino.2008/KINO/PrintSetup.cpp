// PrintSetup.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "PrintSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// PrintSetup dialog
void GetPrintOffs(int type, int& left, int& top)
{
	UINT offs[6];
	CString str = AfxGetApp()->GetProfileString("Print setup", "Offsets");
	if(sscanf(str,"%d %d %d %d %d %d", &offs[0],&offs[1],&offs[2],&offs[3],
		&offs[4],&offs[5]) < 6)
	{
		CFile file;
		if(file.Open("pso.dat",CFile::modeRead) && file.GetLength() == sizeof(offs))
		{
			file.Read(offs, sizeof(offs));
		}else
		{
			memset(offs, 0, sizeof(offs));
		};
	}
	left = offs[type * 2];
	top = offs[type * 2 + 1];
}

PrintSetup::PrintSetup(CWnd* pParent /*=NULL*/)
	: CDialog(PrintSetup::IDD, pParent)
{
	CString str = AfxGetApp()->GetProfileString("Print setup", "Offsets");
	if(sscanf(str,"%d %d %d %d %d %d", &offs[0],&offs[1],&offs[2],&offs[3],
		&offs[4],&offs[5]) < 6)
	{
		CFile file;
		if(file.Open("pso.dat",CFile::modeRead) && file.GetLength() == sizeof(offs))
		{
			file.Read(offs, sizeof(offs));
		}else
		{
			memset(offs, 0, sizeof(offs));
		};
	}
	//{{AFX_DATA_INIT(PrintSetup)
	m_type = -1;
	m_loff = 0;
	m_toff = 0;
	//}}AFX_DATA_INIT
}
PrintSetup::~PrintSetup()
{
	char str[100];
	sprintf(str, "%d %d %d %d %d %d", offs[0], offs[1], offs[2], offs[3],
		offs[4], offs[5]);
	AfxGetApp()->WriteProfileString("Print setup", "Offsets", str);
}


void PrintSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PrintSetup)
	DDX_CBIndex(pDX, IDC_DOCTYPE, m_type);
	DDX_Text(pDX, IDC_LOFF, m_loff);
	DDX_Text(pDX, IDC_TOFF, m_toff);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PrintSetup, CDialog)
	//{{AFX_MSG_MAP(PrintSetup)
	ON_BN_CLICKED(IDC_DEFOFF, OnDefoff)
	ON_BN_CLICKED(IDC_SETDEFOFF, OnSetdefoff)
	ON_CBN_SELCHANGE(IDC_DOCTYPE, OnSelchangeDoctype)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PrintSetup message handlers

void PrintSetup::OnDefoff() 
{
	CFile file;
	if(file.Open("pso.dat",CFile::modeRead) && file.GetLength() == sizeof(offs))
	{
		file.Read(offs, sizeof(offs));
	}else
	{
		memset(offs, 0, sizeof(offs));
	};
	m_loff = offs[m_type * 2];
	m_toff = offs[m_type * 2 + 1];
	UpdateData(FALSE);
}

void PrintSetup::OnSetdefoff() 
{
	UpdateData();
	CFile file;
	if(file.Open("pso.dat",CFile::modeWrite|CFile::modeCreate))
	{
		file.Write(offs, sizeof(offs));
	};
}

void PrintSetup::OnSelchangeDoctype() 
{
	int oldtype = m_type;
	UpdateData();
	if(oldtype >= 0)
	{
		offs[oldtype * 2] = m_loff;
		offs[oldtype * 2 + 1] = m_toff;
	}
	m_loff = offs[m_type * 2];
	m_toff = offs[m_type * 2 + 1];
	UpdateData(FALSE);
}
