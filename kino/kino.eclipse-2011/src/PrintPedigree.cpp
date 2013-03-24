#include "stdx.h"
#include "dblib.h"
#include "ErrorCodes.h"
#include "Application.h"
#include "Exception.h"
#include  <locale.h>
#include "TempRecord.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define mr_left		edt4
#define mr_top		edt5
#define mr_right	edt6
#define mr_bottom	edt7
#define mr_size		cmb2
#define st_size		stc2
#define cb_type		10
#define gr_type		grp1
#define cb_pname	cmb4
UINT ct_hide[] = {rad1, rad2, rad3, stc2, stc3, edt1, edt2};

/*#define ct_hide1	rad1
#define ct_hide2	rad2
#define ct_hide3	rad3
#define ct_hide4	stc2
#define ct_hide5	stc3
#define ct_hide6	edt1
#define ct_hide7	edt2
*/


class Form
{
	HANDLE hFile;
public:
	enum Mode{read, write};
	Form(const char* name, Mode md);
	~Form(){CloseHandle(hFile);};
	UINT Read(void* lpBuf, UINT nCount);
	void Write(void* lpBuf, UINT nCount);
	UINT GetLength();
};

typedef struct PEDIINFO_tag
{
	HDC		hdc;
//	Form*	pForm;
	RKey*	pRKey;
	int		type;
	bool	blank;
//	int		left;
//	int		top;
	UINT	offs[8];
}PEDIINFO;

static int Print(PEDIINFO* pi);
UINT APIENTRY PageSetupHook( HWND hdlg, UINT uiMsg, WPARAM wParam, LPARAM lParam );
UINT APIENTRY PrintHookProc( HWND hdlg, UINT uiMsg, WPARAM wParam, LPARAM lParam );
void GetPrintOffs(UINT* poffs);
void SetPrintOffs(UINT* poffs);
HDC XCreateDC(HGLOBAL hDevNames, HGLOBAL hDevMode);

void PrintPedigree(HWND hWnd, RKey* prk)
{
	if(prk->IsNull())
		return;
	PEDIINFO* pi = new PEDIINFO;
	try{
		{
			TempRecord tr("select ptype from dog where id = ?", prk);
			tr.MoveNext();
			const char* s1 = tr[0];
			pi->type = s1 ? atoi(s1) : -1;
		}
		pi->pRKey = prk;
		GetPrintOffs(pi->offs);
		PRINTDLG pd;
		memset(&pd, 0, sizeof pd);
		pd.lStructSize = sizeof pd;
		pd.hwndOwner = hWnd;
		pd.Flags = PD_ENABLEPRINTHOOK | PD_RETURNDC | PD_USEDEVMODECOPIESANDCOLLATE;
		pd.lpfnPrintHook = PrintHookProc;
		pd.lCustData = (LPARAM)pi;
		if(PrintDlg(&pd))
		{
			if(pi->blank)
			{
				char stmt[80];
				sprintf(stmt, "INSERT INTO PrintLog(ptype, dog_id) VALUES (%d, ?)", pi->type);
				Record::pDB->Prepare(stmt);
				Record::pDB->BindParameter(prk);
				Record::pDB->Exec();
				Record::pDB->FlushEx();
			}
			pi->hdc = pd.hDC;//XCreateDC(psd.hDevNames, psd.hDevMode);
			Print(pi);
		}
		
	}catch(Exception* e)
	{
		e->Effect();
	}
	delete pi;
}

void PrintPedigreeSetup(HWND hWnd, RKey* prk)
{
	PEDIINFO* pi = new PEDIINFO;
	try{
		pi->type = 0;
		pi->pRKey = prk;
		GetPrintOffs(pi->offs);
		PAGESETUPDLG psd;
		memset(&psd, 0, sizeof psd);
		psd.lStructSize = sizeof psd;
		psd.hwndOwner = hWnd;
		psd.Flags = PSD_DEFAULTMINMARGINS | PSD_DISABLEORIENTATION | 
			PSD_MARGINS | PSD_ENABLEPAGESETUPHOOK;
		psd.lpfnPageSetupHook = PageSetupHook;
		psd.rtMargin.left =	pi->offs[pi->type * 2];
		psd.rtMargin.top =	pi->offs[pi->type * 2 + 1];
		psd.lCustData = (LPARAM)pi;
		if(PageSetupDlg(&psd))
			SetPrintOffs(pi->offs);
//		pi->hdc = XCreateDC(psd.hDevNames, psd.hDevMode);
//		Print(pi);
	}catch(Exception* e)
	{
		e->Effect();
	}
	delete pi;
}
//	DWORD id;
//	CreateThread(NULL, 0, (PTHREAD_START_ROUTINE)Print, pi, 0, &id);
#pragma optimize( "g", off)
void GetPdg_rc(int i, int& r, int& c)
{
	r = c = 0;
	r = (r = i & 16)>>4 | (~(r=r>>1) & (r |= i & 8)) >> 2 | (~r>>1 & i & 4);
	c = ((c = i & 16)>>4)*3 + ((~(c=c>>1) & (c |= i & 8)) >> 2) + ((~c>>1 & i & 4)>>2) - 1;
}

void print_pedigree(RECT* pdg, PEDIINFO* pi)
{
	RECT rect;
	char stmt[50];
	sprintf(stmt, "{CALL  sp_pedigree(?, 1, %d)}", pi->type ? 1 : 0);
	TempRecord tr(stmt, pi->pRKey);
	SIZE size = {pdg[0].right - pdg[0].left, pdg[0].bottom - pdg[0].top};
	int roh = size.cy / 16;
	int coh[3] = {size.cx * 5 / 19,size.cx * 6 / 19,size.cx * 8 / 19};
	int col[3] = {0, coh[0], coh[0]+coh[1]};
	UINT nID = 0;
	const char* s1;
	int r = 0;
	int c = 0;
	while(tr.MoveNext())
	{
		nID = atoi(tr[0]);
		ASSERT(nID>=0 && nID < 30);
		if(nID == 0 || nID == 1)
			rect = pdg[nID + 1];
		else
		{
			nID += 2;
			GetPdg_rc(nID, r, c);
			rect.left = pdg[0].left + col[c];
			rect.top = pdg[0].top + ((nID << (2 - c)) & 15) * roh;
			rect.right = rect.left + coh[c];
			rect.bottom = rect.top + r * roh;
		}
		s1 = tr[1];
		if(s1)
		{
			DrawText(pi->hdc, s1, strlen(s1), &rect, DT_TOP|DT_LEFT|DT_WORDBREAK);
		}
	}
}

static int Print(PEDIINFO* pi)
{
	const char* frm = NULL;
	switch(pi->type)
	{
	case 0: frm = "ipedi.fr";break;
	case 1: frm = "upedi.fr";break;
	case 2: frm = "rpedi.fr";break;
	case 3: frm = "spedi.fr";break;
	default: return 0;
	}
	
	Form file(frm, Form::read);
	int left = pi->offs[pi->type * 2];
	int top = pi->offs[pi->type * 2 + 1];
	DOCINFO di;
	di.cbSize = sizeof(DOCINFO);
	di.lpszDocName = "Родословная";
	di.lpszOutput = NULL;
	StartDoc(pi->hdc, &di);
	StartPage(pi->hdc);
	SaveDC(pi->hdc);
	SetMapMode(pi->hdc, MM_HIMETRIC);
	SetWindowOrgEx(pi->hdc, -left,top, NULL);

	LOGFONT lf;
	HFONT hFont;
	file.Read(&lf, sizeof(lf));
	HDC hDC = ::GetDC(NULL);
	int scry = ::GetDeviceCaps(hDC, LOGPIXELSY);
	ReleaseDC(NULL, hDC);
	lf.lfHeight = lf.lfHeight * 2540 /scry;
	hFont = CreateFontIndirect(&lf);
	SelectObject(pi->hdc, hFont);

	int fields;
	file.Read(&fields, sizeof(fields));
	RECT rect;

	int buflen = 30;
	int outbuflen = 0;
	char* buf = new char[buflen];
	char* outbuf = NULL; 
	int strlena = 0;
	RECT pdg[3];
	char stmt[50];
	sprintf(stmt, "{CALL  sp_pedigreeheader(?, %d)}", pi->type);
	{
		TempRecord tr(stmt, pi->pRKey);
		tr.MoveNext();
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
					pdg[atoi(s1)] = rect;
				else
					pdg[0] = rect;
			}else
			{
				const char* s1 = tr[buf];
				if(s1)
					DrawText(pi->hdc, s1, strlen(s1), &rect, DT_TOP|DT_LEFT|DT_WORDBREAK);
			}
			
		}
	}
	delete buf;
	print_pedigree(pdg, pi);

//	delete pi->pForm;
	RestoreDC(pi->hdc, -1);
	EndPage(pi->hdc);
	EndDoc(pi->hdc);
	return 0;
}

UINT APIENTRY PrintHookProc( HWND hdlg, UINT uiMsg, WPARAM wParam, LPARAM lParam )
{
	switch(uiMsg)
	{
	case WM_INITDIALOG:
		{
			PEDIINFO* pi = ((PEDIINFO*)((PRINTDLG*)lParam)->lCustData);
			ASSERT(pi);
			SetWindowLong(hdlg, DWL_USER, (long)pi);
			int num = _countof(ct_hide);
			while(num--)
				ShowWindow(::GetDlgItem(hdlg, ct_hide[num]), SW_HIDE);
			SetWindowText(::GetDlgItem(hdlg, gr_type), "Тип родословной");
			RECT r;
			GetWindowRect(GetDlgItem(hdlg, gr_type), &r);
			SIZE sz = {r.right - r.left - 15, r.bottom - r.top};
			ScreenToClient(hdlg, (POINT*)&r);
			HWND hType = CreateWindow("COMBOBOX", NULL, CBS_DROPDOWNLIST|
				WS_CHILD|WS_VISIBLE|WS_TABSTOP,
				r.left + 7, r.top + 0x15, sz.cx, sz.cy, hdlg, (HMENU) cb_type,
				(HANDLE)GetWindowLong(hdlg, GWL_HINSTANCE), NULL);

			HWND hBlank = CreateWindow("BUTTON", "На бланке", BS_AUTOCHECKBOX|BS_TEXT|
				WS_CHILD|WS_VISIBLE|WS_TABSTOP,
				r.left + 0x10, r.top + 0x30, sz.cx - 0x20, 0x20, hdlg, (HMENU) (cb_type + 1),
				(HANDLE)GetWindowLong(hdlg, GWL_HINSTANCE), NULL);
			HWND hPn = ::GetDlgItem(hdlg, cb_pname);
			SetWindowPos(hType, hPn, -1, -1, -1, -1, SWP_NOREDRAW|
				SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
			SetWindowPos(hBlank, hType, -1, -1, -1, -1, SWP_NOREDRAW|
				SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
			 
 

			WPARAM hFont = ::SendMessage(hPn, WM_GETFONT, 0, 0);
			::SendMessage(hType, WM_SETFONT, hFont, MAKELPARAM(TRUE, 0));
			::SendMessage(hBlank, WM_SETFONT, hFont, MAKELPARAM(TRUE, 0));

			::SendMessage(hType, CB_ADDSTRING, 0, (LPARAM)"Международная");
			::SendMessage(hType, CB_ADDSTRING, 0, (LPARAM)"Национальная");
			::SendMessage(hType, CB_ADDSTRING, 0, (LPARAM)"Реестровая");
			::SendMessage(hType, CB_ADDSTRING, 0, (LPARAM)"Бланк регистрации");
			::SendMessage(hType, CB_SETCURSEL, pi->type, 0);
		}
		break;
	case WM_COMMAND:
		if(wParam == IDOK)
		{
			PEDIINFO* pi = (PEDIINFO*)GetWindowLong(hdlg, DWL_USER);
			pi->blank = ::SendMessage(::GetDlgItem(hdlg, cb_type + 1),
				BM_GETCHECK, 0, 0) > 0;
			pi->type = ::SendMessage(GetDlgItem(hdlg, cb_type),
				CB_GETCURSEL, lParam, 0);
		}
		break;
	}
	return 0;
}
UINT APIENTRY PageSetupHook( HWND hdlg, UINT uiMsg, WPARAM wParam, LPARAM lParam )
{
	switch(uiMsg)
	{
	case WM_INITDIALOG:
		{
			EnableWindow(GetDlgItem(hdlg, mr_right), FALSE);
			EnableWindow(GetDlgItem(hdlg, mr_bottom), FALSE);
			SetWindowText(GetDlgItem(hdlg, st_size), "Тип");
			PEDIINFO* pi = ((PEDIINFO*)((PAGESETUPDLG*)lParam)->lCustData);
			SetWindowLong(hdlg, DWL_USER, (long)pi);
			
			HWND hSz = GetDlgItem(hdlg, mr_size);
			RECT r;
			GetWindowRect(hSz, &r);
			SIZE sz = {r.right - r.left, r.bottom - r.top};
			ScreenToClient(hdlg, (POINT*)&r);
			ShowWindow(hSz, SW_HIDE);
			HWND hType = CreateWindow("COMBOBOX", NULL, CBS_DROPDOWNLIST|
				WS_CHILD|WS_VISIBLE|WS_TABSTOP,
				r.left, r.top, sz.cx, sz.cy * 4, hdlg, (HMENU) cb_type,
				(HANDLE)GetWindowLong(hdlg, GWL_HINSTANCE), NULL);
			::SetWindowPos(HWND_TOP, hType, -1, -1, -1, -1, SWP_NOREDRAW|
				SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
			SetFocus(hType);
			::SendMessage(hType, WM_SETFONT,
				::SendMessage(hSz, WM_GETFONT, 0, 0), MAKELPARAM(TRUE, 0));
			::SendMessage(hType, CB_ADDSTRING, 0, (LPARAM)"Международная");
			::SendMessage(hType, CB_ADDSTRING, 0, (LPARAM)"Национальная");
			::SendMessage(hType, CB_ADDSTRING, 0, (LPARAM)"Реестровая");
			::SendMessage(hType, CB_ADDSTRING, 0, (LPARAM)"Бланк регистрации");
			::SendMessage(hType, CB_SETCURSEL, pi->type, 0);
		}
		break;
	case WM_COMMAND:
		if((HIWORD(wParam) == CBN_SELCHANGE && ((int) LOWORD(wParam)) == cb_type) ||
			(wParam == IDOK))
		{
			char* lc = setlocale(LC_ALL, "rus");
			PEDIINFO* pi = (PEDIINFO*)GetWindowLong(hdlg, DWL_USER);
			int type = ::SendMessage(GetDlgItem(hdlg, cb_type),
				CB_GETCURSEL, lParam, 0);
			if(type >= 0)
			{
				char str[10];
				::GetDlgItemText(hdlg, mr_left, str, 10);
				pi->offs[pi->type * 2] = (UINT)(atof(str) * 100);
				::GetDlgItemText(hdlg, mr_top, str, 10);
				pi->offs[pi->type * 2 + 1] = (UINT)(atof(str) * 100);
				pi->type = type;
				sprintf(str, "%gmm", ((double)pi->offs[pi->type * 2]) /100);
				::SetDlgItemText(hdlg, mr_left, str);
				sprintf(str, "%gmm", ((double)pi->offs[pi->type * 2 + 1]) /100);
				::SetDlgItemText(hdlg, mr_top, str);
			}

			setlocale(LC_ALL, lc);
		}
		break;
	}
	return 0;
} 

Form::Form(const char* name, Mode md)
{
	hFile = CreateFile(name, md == read ? GENERIC_READ : GENERIC_WRITE,
		0, NULL, md == read ? OPEN_EXISTING : OPEN_ALWAYS,
		FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		Exception::ex.printf(MSG_ERROR_FILE_NOTFOUND, name);
//		Exception::ex.SetDelMem(this);
		throw &Exception::ex;
	}
}

UINT Form::GetLength()
{
	return GetFileSize(hFile, NULL);
}

UINT Form::Read( void* lpBuf, UINT nCount )
{
	DWORD dw;
	if(ReadFile(hFile, lpBuf, nCount, &dw, NULL))
		return dw;
	else
		return 0;

}

void Form::Write( void* lpBuf, UINT nCount )
{
	DWORD dw;
	WriteFile(hFile, lpBuf, nCount, &dw, NULL);
}

void SetPrintOffs(UINT* offs)
{
	char str[80];
	sprintf(str,"%d %d %d %d %d %d %d %d", offs[0], offs[1], offs[2], offs[3],
		offs[4], offs[5], offs[6], offs[7]);
	Application::GetApp()->WriteProfileString( "Print setup", "Offsets", str);
}

void GetDefPrintOffs(UINT* offs)
{
	UINT size = sizeof(UINT) * 8;
	memset(offs, 0, size);
	try{
		Form file("pso.dat", Form::read);
		if(file.GetLength() == size)
			file.Read(offs, size);
	}catch(Exception*){};
}

void SetDefPrintOffs(UINT* offs)
{
	UINT size = sizeof(UINT) * 8;
	try{
		Form file("pso.dat", Form::write);
		file.Write(offs, size);
	}catch(Exception*){};
}

void GetPrintOffs(UINT* offs)
{
	const char* str = Application::GetApp()->GetProfileString("Print setup", "Offsets");
	if(!str || sscanf(str,"%d %d %d %d %d %d %d %d", &offs[0],&offs[1],&offs[2],&offs[3],
		&offs[4],&offs[5], &offs[6],&offs[7]) < 8)
		GetDefPrintOffs(offs);
	if(str)
		delete (char*)str;
}
HDC XCreateDC(HGLOBAL hDevNames, HGLOBAL hDevMode)
{
	if (hDevNames == NULL)
		return NULL;

	LPDEVNAMES lpDevNames = (LPDEVNAMES)::GlobalLock(hDevNames);
	LPDEVMODE  lpDevMode = (hDevMode != NULL) ?
						(LPDEVMODE)::GlobalLock(hDevMode) : NULL;

	if (lpDevNames == NULL)
		return NULL;

	HDC hDC = ::CreateDC((LPCTSTR)lpDevNames + lpDevNames->wDriverOffset,
					  (LPCTSTR)lpDevNames + lpDevNames->wDeviceOffset,
					  (LPCTSTR)lpDevNames + lpDevNames->wOutputOffset,
					  lpDevMode);

	::GlobalUnlock(hDevNames);
	if (hDevMode != NULL)
		::GlobalUnlock(hDevMode);
	return hDC;
}
/*
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
*/