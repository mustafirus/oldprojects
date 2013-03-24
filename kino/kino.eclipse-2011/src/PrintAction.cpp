// PrintAction.cpp: implementation of the PrintAction class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RField.h"
#include "DataBase.h"
#include "Record.h"
#include "Resource.h"
#include "RKey.h"
#include "Field.h"
#include "SelectAction.h"
#include "Pedigree.h"
#include "Exception.h"
#include "PrintSetup.h"
#include "PrintAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PrintAction::PrintAction(Action* pParent, RKey* key) : Action(pParent),
pRKey(key), pDC(NULL), pFont(0), pnum(-1), lang(0)
{
	char* str = "<S Международная, Национальная, Реестровая>";
	SelectAction* psa = new SelectAction(this, str);
}

PrintAction::~PrintAction()
{
	if(pDC)
	{
		pDC->Detach();
		delete pDC;
	}
	if(pFont)
		delete pFont;
/*	if(rects)
		delete[] rects;
*/
}

void PrintAction::Return(Action* pa, bool success/* = false*/)
{
/*	if(!success)
	{
		delete pa;
		Return(false);
		return;
	}
	int sel = ((SelectAction*)pa)->GetSelection();
	delete pa;
	GetPrintOffs(sel, left, top);
	switch(sel)
	{
	case 0:
		if(!file.Open("ipedi.fr", CFile::modeRead))
			Except("File upedi.fr - not found.");
		lang = 1;
		break;
	case 1:
		if(!file.Open("upedi.fr", CFile::modeRead))
			Except("File upedi.fr - not found.");
		break;
	case 2:
		if(!file.Open("rpedi.fr", CFile::modeRead))
			Except("File upedi.fr - not found.");
		break;
	default:
		Return(false);
		return;
	};
*/
	CPrintDialog dlg(FALSE);
	if(dlg.DoModal() == IDOK && dlg.m_pd.hDC)
	{
		pDC = new CDC;
		pDC->Attach(dlg.m_pd.hDC);
	}else
	{
		Return(false);
		return;
	}
	DWORD id;
	CreateThread(NULL, 0, (PTHREAD_START_ROUTINE)Run, this, 0, &id);
};

void PrintAction::Print()
{
	DOCINFO di;
	di.cbSize = sizeof(DOCINFO);
	di.lpszDocName = "Родословная";
	di.lpszOutput = NULL;
	pDC->StartDoc(&di);
	pDC->StartPage();
	pDC->SaveDC();
	pDC->SetMapMode(MM_HIMETRIC);
	pDC->SetWindowOrg(-left,top);
/*	pDC->SetViewportOrg(- pDC->GetDeviceCaps(PHYSICALOFFSETX),
		- pDC->GetDeviceCaps(PHYSICALOFFSETY));
*/
	if(pFont)
		delete pFont;
		LOGFONT lf;
	pFont = new CFont;
	file.Read(&lf, sizeof(lf));
	HDC hDC = ::GetDC(NULL);
	int scry = ::GetDeviceCaps(hDC, LOGPIXELSY);
	ReleaseDC(NULL, hDC);
	lf.lfHeight = lf.lfHeight * 2540 /scry;
	pFont->CreateFontIndirect(&lf);
	pDC->SelectObject(pFont);

	int fields;
	file.Read(&fields, sizeof(fields));
//	RECT* rects = new RECT[fields];
	RECT rect;

	int buflen = 30;
	int outbuflen = 0;
	char* buf = new char[buflen];
	char* outbuf = NULL; 
	int strlena = 0;
	Pedigree pdg(pRKey, true, lang);
	for(int i=0; i < fields; i++)
	{
		file.Read(&rect, sizeof(RECT));
		file.Read(&strlena, sizeof(char));
		if(strlena > buflen)
		{
			delete buf;
			buf = new char[buflen = strlena+1];
		}
		file.Read(buf, strlena);
		buf[strlena] = 0;
		if(!strnicmp(buf, "pedigree",8))	// Print Pedigree
		{
			char* s1 = strchr(buf, ':');
			if(s1++)
			{
				pdg.SetRectDog(atoi(s1), &rect);
			}else
			{
				pdg.SetSize(rect.right-rect.left, rect.bottom-rect.top);
				pdg.SetSizes();
				pdg.SetPos(*((POINT*)&rect));
			}
		}else if(!strnicmp(buf, "stud",4))
		{
			char stud[30];
			int id = atoi(pRKey->GetSrcData());
			if(Record::pDB->ExecProc("sp_stud", id, stud, 30))
				pDC->DrawText(stud, -1, &rect, DT_LEFT|DT_WORDBREAK);
		}else
		{
			char* fmt = NULL;
			char* rec = strchr(buf,':');
			if(rec)
			{
				*rec++ = 0;
				rec += strspn(rec, " ");
				fmt = buf;
			}
			else
				rec = buf;
			RFields rf;
			if(*rec)
			{
				pRKey->GetRecord()->GetRFields(rec, rf);
			}
			int outlen = 0;
			for(int i = 0; i<rf.GetCount(); i++)
			{
				outlen+=rf[i]->GetField()->scrlen;
			}
			outlen += rf.GetCount() + 1	+ (fmt ? (int)strlen(fmt) : 0);
			if(outlen > outbuflen)
			{
				if(outbuf)
					delete outbuf;
				outbuf = new char[outbuflen = outlen];
				*outbuf=0;
			}
			if(fmt)
			{
				int i=0;
				char* s1 = outbuf;
				while(*fmt)
				{
					if(*fmt == '%' && *++fmt=='s')
					{
						if(i<rf.GetCount())
						{
							strcpy(s1, rf[i++]->GetPtr());
							s1 += strlen(s1);
						}
						fmt++;
					}
					else
						*s1++ = *fmt++;
				}
				*s1=0;
			}else
			{
				for(i = 0; i<rf.GetCount(); i++)
				{
					strcat(outbuf, rf[i]->GetPtr());
				}
			}
			pDC->DrawText(outbuf, -1, &rect, DT_LEFT|DT_WORDBREAK);
			*outbuf= 0;
		}
	}
	pdg.Print(pDC);
	pDC->RestoreDC(-1);
	pDC->EndPage();
	pDC->EndDoc();
	Return(true);
}

int PrintAction::Run(PrintAction* pa)
{
	if(!pa->pDC)
	{
		pa->Return(false);
		return -1;
	}
	pa->Print();
	return 0;
}
