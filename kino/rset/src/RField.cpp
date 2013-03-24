#if 0
#include "stdx.h"
#include "Dict.h"
#include "RKey.h"
#include "DataBase.h"
#include "Record.h"
#include "RField.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
RField::RField() :
buf(NULL), type(Field::Char), pQField(NULL), pPRField(NULL), pRec(NULL), pind(NULL),
/*state(s_dummy), */buf_attrib(b_none), /*attr(Hidden), */v(0){}

RField::RField(QField* pqf, Record* pr, RField* prf) :
buf(NULL), type(Field::Char), pQField(pqf), pPRField(prf), pRec(pr), pind(NULL),
/*state(s_dummy), */buf_attrib(b_none), /*attr(Hidden), */v(0)
{
	if(!prf)
	{
		buf		= new char[GetLen()];
		pind	= Record::pDB->GetIndicator(1);
		*buf	= 0;
		buf_attrib = b_native;
	}else
	{
		buf		= NULL;
		buf_attrib = b_foreign;
	}
}

RField::~RField()
{
	if(buf_attrib == b_native && buf)
		delete[] buf;
	if(pind)
		delete pind;
}

RField::operator const char*()
{
	if(state == s_null)
		return NULL;
	if(state != s_data)
		pRec->Load();
	return buf;
}

void RField::SetData(bool null)
{
	v++;
	state = s_data;
	if(null)
		state = s_null;
	else
		state /= s_null;
	state /= s_modified;
	if(state != s_null)
	{
		if(type != Field::Char)
			type = Record::pDB->Convert(buf, buf, type, Field::Char);
		if(((Field*)*pQField)->deft)
		{
			if(!strncmp(buf, ((Field*)*pQField)->deft, strlen(buf)))
				state = s_null;
		}
	}
};

void RField::SetDefault()
{
	Field* pf = *pQField;
	if(pf->deft)
	{
		strcpy(buf, pf->deft);
		state = s_modified;
	}
}

int RField::GetLen()
{
	return ((Field*)*pQField)->strlen+1;
}

bool RField::ro()
{
	return pQField->ro() || !((QTable*)*pQField)->IsMaster();
};

RField::operator RKey*()
{
	return pRec->GetRKey(*pQField);
}

void RField::Delete()
{
	if(ro())
	{
		RKey* prk = pRec->GetRKey(*this);
		state = s_modified;
		state = s_null;
		v++;
		pRec->SetModified();
		prk->SetNull();
		pRec->Refresh(prk);
	}else
	{
		state = s_null;
		state = s_modified;
		state = s_data;
		v++;
	}
}

void RField::OnCopy(HWND hWnd)
{
	RKey* prk = pRec->GetRKey(*pQField);
	ASSERT(prk);
	if(prk->IsNull())
		return;
	if(!(OpenClipboard(hWnd) &&	EmptyClipboard()))
	{
		CloseClipboard();
		return;
	}
	HANDLE h = GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE, 256);
	prk->Get((char*)GlobalLock(h));
	GlobalUnlock(h);
	SetClipboardData(Record::CF_RKEY, h);

	const char* s1 = *this;
	int len = s1 ? strlen(s1) + 1 : 0;
	if(len)
	{
		h = GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE, len * 2);
		MultiByteToWideChar(CP_ACP, 0, s1, len, (LPWSTR)GlobalLock(h), len * 2);
		GlobalUnlock(h);
		SetClipboardData(CF_UNICODETEXT, h);
		
		h = GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE, len);
		memcpy(GlobalLock(h), s1, len);
		GlobalUnlock(h);
		SetClipboardData(CF_TEXT, h);
	}
	CloseClipboard();
}

void RField::OnPaste(HWND hWnd)
{
	if(!(OpenClipboard(hWnd) && IsClipboardFormatAvailable(Record::CF_RKEY)))
	{
		CloseClipboard();
		return;
	}
	char* str = (char* )GetClipboardData(Record::CF_RKEY);
	CloseClipboard();
	RKey* prk = pRec->GetRKey(*pQField);
	if(!prk->CanSet(str))
		return;
	prk->Set(str);
	pRec->Refresh(prk);
}

bool RField::CanCopy()
{
	RKey* prk = pRec->GetRKey(*pQField);
	return !prk->IsNull();
}

bool RField::CanPaste(HWND hWnd)
{
	RKey* prk = pRec->GetRKey(*pQField);
	ASSERT(prk);
	bool can = false;
	if(OpenClipboard(hWnd) && IsClipboardFormatAvailable(Record::CF_RKEY))
	{
		HANDLE h = GetClipboardData(Record::CF_RKEY);
		char* str = (char*)GlobalLock(h);
		can = prk->CanSet(str);
		GlobalUnlock(h);
	}
	CloseClipboard();
	return can;
}
#endif
