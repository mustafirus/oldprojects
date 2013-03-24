#include "stdafx.h"
#include "Wnd.h"
#include "Application.h"

Application* Application::pApp = NULL;

Application::Application( HINSTANCE	hi ): hInstance(hi), pMainWnd(NULL){
	ASSERT(pApp == NULL);pApp = this;
	Thread::attach();
	hProcess = ::GetCurrentProcess();
	nProcessID = ::GetCurrentProcessId();
}

Application::~Application(){
	Thread::detach();
	CloseHandle(hProcess);
	hProcess = NULL;
	nProcessID = 0;
}

bool Application::onIdle(){
	pMainWnd->SendMessageToDescendants(WM_IDLEUPDATECMDUI);
	return false;
}

bool Application::preTranslateMessage(MSG* pMsg){
	return pMainWnd && pMainWnd->preTranslateMessage(pMsg) || false;
}

char* Application::LoadString(UINT sID)
{
	// try fixed buffer first (to avoid wasting space in the heap)
	char temp[256];//
	int nLen = ::LoadString(hInstance, sID, temp, sizeof(temp));
	if(!nLen)
		return NULL;
	if (sizeof(temp) - nLen > 1)
	{
		return strdup(temp);
	}

	// try buffer size of 512, then larger size until entire string is retrieved
	int nSize = 256;
	char* buf = NULL;
	do
	{
		nSize += 256;
		buf = (char*)realloc(buf, nSize);
		nLen = ::LoadString(hInstance, sID, buf, nSize);
	} while (nSize - nLen <= 1);
	char* s1 = strdup(buf);
	free(buf);
	return s1;
}

