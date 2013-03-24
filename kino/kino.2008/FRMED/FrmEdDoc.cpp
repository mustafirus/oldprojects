// FrmEdDoc.cpp : implementation of the CFrmEdDoc class
//

#include "stdafx.h"
#include "FrmEd.h"

#include "FrmEdDoc.h"
#include "FrmEdView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFrmEdDoc

IMPLEMENT_DYNCREATE(CFrmEdDoc, CDocument)

BEGIN_MESSAGE_MAP(CFrmEdDoc, CDocument)
	//{{AFX_MSG_MAP(CFrmEdDoc)
	ON_COMMAND(ID_FILE_CHANGE_BMP, OnFileChangeBmp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFrmEdDoc construction/destruction

CFrmEdDoc::CFrmEdDoc() : pFont(0)
{
	// TODO: add one-time construction code here

}

CFrmEdDoc::~CFrmEdDoc()
{
}

BOOL CFrmEdDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	CFileDialog dlg(TRUE, "bmp", NULL, 0, "Bitmaps (*.bmp)|*.bmp||");
	CFile file;
	if(!dlg.DoModal() == IDOK)
		return FALSE;
	if(!file.Open(dlg.GetPathName(), CFile::modeRead))
		return FALSE;
	Bk.LoadDIB(&file);
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CFrmEdDoc serialization

void CFrmEdDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		POSITION pos = GetFirstViewPosition();
		((CFrmEdView*)GetNextView(pos))->Save();
		LOGFONT lf;
		memset(&lf, 0, sizeof(lf));
		if(pFont)
			pFont->GetLogFont(&lf);
		ar.Write(&lf, sizeof(lf));
		int n = frect.GetSize();
		ar << n;
		for(int i=0; i<n; i++)
		{
			RECT* pr = frect[i];
			ar.Write(pr, sizeof(RECT));
			ar<<(*ftext[i]);
		}

	}
	else
	{
		LOGFONT lf;
		if(pFont)
			delete pFont;
		pFont = new CFont;
		ar.Read(&lf, sizeof(lf));
		pFont->CreateFontIndirect(&lf);
		int n = frect.GetSize();
		ar >> n;
		for(int i=0; i<n; i++)
		{
			frect.Add(new CRect);
			ftext.Add(new CString);
			RECT* pr = frect[i];
			ar.Read(pr, sizeof(RECT));
			ar>>(*ftext[i]);
			afxDump << *pr << *ftext[i];
		}
	}
	Bk.Serialize(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CFrmEdDoc diagnostics

#ifdef _DEBUG
void CFrmEdDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFrmEdDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFrmEdDoc commands

void CFrmEdDoc::OnFileChangeBmp() 
{
	CFileDialog dlg(TRUE, "bmp", NULL, OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, "Bitmaps *.bmp|*.bmp||");
	if(dlg.DoModal()==IDOK)
	{
		CFile fp(dlg.GetFileName(), CFile::modeRead);
		Bk.LoadDIB(&fp);
	}

}
