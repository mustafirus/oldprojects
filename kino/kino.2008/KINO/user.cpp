#include "stdx.h"
#include "resource.h"
static char* us;
static int nu;
static char* ps;
static int np;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
static BOOL DlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(uMsg == WM_COMMAND)
	{
		int idButton = (int) LOWORD(wParam); 
		if(idButton == IDOK || idButton == IDCANCEL)
		{
			GetDlgItemText(hwndDlg, IDC_USER, us, nu);
			GetDlgItemText(hwndDlg, IDC_PASS, ps, np);
			return EndDialog(hwndDlg, idButton);
		}
	}
#ifdef _DEBUG
	if(uMsg == WM_INITDIALOG)
	{
		SetFocus(hwndDlg);
		SetDlgItemText(hwndDlg, IDC_USER, "sa");
	}
#endif //_DEBUG
	return FALSE;
}

bool GetUser(char* user, int userlen, char* pass, int passlen, HINSTANCE resourse_hInst, HWND hWndMain)
{
	us=user;
	ps=pass;
	nu=userlen;
	np=passlen;
	int ret = DialogBox(resourse_hInst, MAKEINTRESOURCE(IDD_LOGIN),
		hWndMain,(DLGPROC)DlgProc);
	return (ret == IDOK)?true:false;
}
