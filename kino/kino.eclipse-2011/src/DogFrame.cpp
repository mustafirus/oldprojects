// DogFrame.cpp: implementation of the DogFrame class.
//
//////////////////////////////////////////////////////////////////////

#include "stdx.h"
/*#include "Record.h"
#include "DataBase.h"
*/
#include "dblib.h"
#include "Action.h"
#include "kinores.h"
#include "DogFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DogFrame::DogFrame(RKey* prk, Record* pr, const char* id, const char* sex, Wnd* pWndParent, UINT nID) :
EditControl(pWndParent, ES_MULTILINE, nID), pRKey(prk),	pRec(pr), id(NULL), sex(NULL), pStMdfr(NULL)
{
	DogFrame::id = sdup(id);
	DogFrame::sex = sdup(sex);
}

/*void DogFrame::SetData(const char* str)
{
	SetWindowText(str);
}
*/
void DogFrame::OnKey(UINT vk, bool fDown, int cRepeat, UINT flags)
{
	switch(vk)
	{
	case VK_RETURN:
		{
			if(GetKeyState(VK_CONTROL)&0x8000)
				Activate(VK_CONTROL);
			else if(GetKeyState(VK_SHIFT)&0x8000)
				Activate(VK_SHIFT);
			else
				Activate(0);
			return;
		}
	default:
		ResendMessage(::GetParent(*this));
//		EditControl::OnKey(vk, fDown, cRepeat, flags);
	}
}

void DogFrame::Activate(UINT syskey)
{
	UINT nID = ID_FORM_CARD;
	switch(syskey)
	{
	case VK_CONTROL:
		nID |= ACT_FLAG_THREAD;
		break;
	}
	if(id)
	{
		pRKey->Set(id);
		Action::GetAction()->Exec(((FrameWnd*)GetParent()), pRec, NULL, nID);
	}
}

//: focused(false), sex(false), id(0), pStMdfr(new StMdfr(NULL, 2,3)){};
/*bool DogFrame::GetDog(int id, int lines, int lang, CString& str, char& s, int* parents)
{
	try{
	return Record::pDB->GetDog(id, lines, lang, str, s, parents);
	}catch(DbException*)
	{
		Record::pDB->Flush();
		return false;
	}
}

void DogFrame::OnDraw(HDC hDC)
{
	CRect r = GetRect();
	pDC->Border(r);
	r.DeflateRect(2,2);
	pDC->DrawText(r, str);
}

void DogFrame::OnPrint(CDC* pDC)
{
	CRect r = GetRect();
	r.DeflateRect(2,2);
	pDC->DrawText(str, -1, r, DT_LEFT|DT_WORDBREAK);
}
*/