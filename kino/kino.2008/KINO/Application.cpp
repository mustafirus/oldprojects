// Application.cpp: implementation of the Application class.
//
//////////////////////////////////////////////////////////////////////

#include "stdx.h"
#include "Wnd.h"
#include "Application.h"

const char szSoftware[] = "Software";
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
Application* Application::pApp = NULL;

void Application::Attach()
{
	Thread::Attach();
	hProcess = ::GetCurrentProcess();
	nProcessID = ::GetCurrentProcessId();
}

void Application::Detach()
{
	Thread::Detach();
	CloseHandle(hProcess);
	hProcess = NULL;
	nProcessID = 0;
}

bool Application::OnIdle()
{
	pMainWnd->SendMessageToDescendants(WM_IDLEUPDATECMDUI);
	return false;
}

bool Application::PreTranslateMessage(MSG* pMsg)
{
	if(WinThread::PreTranslateMessage(pMsg))
		return true;
	return pMainWnd->PreTranslateMessage(pMsg);
}
//////////////////////////////////////////////////////////////////////
// WinMain
//////////////////////////////////////////////////////////////////////
int WINAPI WinMain( HINSTANCE hInstance, 
				   HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	DBG_INIT;
	int nReturnCode;
	Application* pApp = Application::GetApp();
	pApp->Attach();
	pApp->hInstance = hInstance;
	pApp->pCmdLine = sdup(lpCmdLine);
	pApp->nCmdShow = nCmdShow;
	nReturnCode = pApp->Run();
	pApp->Detach();
	return nReturnCode;
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
		return sdup(temp);
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
	char* s1 = sdup(buf);
	free(buf);
	return s1;
}

bool Application::WriteProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry,
			LPCTSTR lpszValue)
{
	ASSERT(lpszSection != NULL);
	if (pszRegistryKey != NULL)
	{
		LONG lResult;
		if (lpszEntry == NULL) //delete whole section
		{
			HKEY hAppKey = GetAppRegistryKey();
			if (hAppKey == NULL)
				return false;
			lResult = ::RegDeleteKey(hAppKey, lpszSection);
			RegCloseKey(hAppKey);
		}
		else if (lpszValue == NULL)
		{
			HKEY hSecKey = GetSectionKey(lpszSection);
			if (hSecKey == NULL)
				return false;
			// necessary to cast away const below
			lResult = ::RegDeleteValue(hSecKey, (LPTSTR)lpszEntry);
			RegCloseKey(hSecKey);
		}
		else
		{
			HKEY hSecKey = GetSectionKey(lpszSection);
			if (hSecKey == NULL)
				return false;
			lResult = RegSetValueEx(hSecKey, lpszEntry, NULL, REG_SZ,
				(LPBYTE)lpszValue, (lstrlen(lpszValue)+1)*sizeof(TCHAR));
			RegCloseKey(hSecKey);
		}
		return lResult == ERROR_SUCCESS;
	}
	return false;
}

const char* Application::GetProfileString( const char* szSection, const char* szEntry, const char* szDefault /*= NULL */)
{
	ASSERT(szSection != NULL);
	ASSERT(szEntry != NULL);
	if (pszRegistryKey != NULL)
	{
		HKEY hSecKey = GetSectionKey(szSection);
		if (hSecKey == NULL)
			return szDefault;
		char* strValue;
		DWORD dwType, dwCount;
		LONG lResult = RegQueryValueEx(hSecKey, (char*)szEntry, NULL, &dwType,
			NULL, &dwCount);
		if(dwCount == 0)
			return szDefault;
		if (lResult == ERROR_SUCCESS)
		{
			ASSERT(dwType == REG_SZ);
			strValue = new char[dwCount];
			lResult = RegQueryValueEx(hSecKey, (LPTSTR)szEntry, NULL, &dwType,
				(LPBYTE)strValue, &dwCount);
		}
		RegCloseKey(hSecKey);
		if (lResult == ERROR_SUCCESS)
		{
			ASSERT(dwType == REG_SZ);
			return strValue;
		}
		return szDefault;
	}
	else
		return szDefault;
}

HKEY Application::GetSectionKey(LPCTSTR lpszSection)
{
	ASSERT(lpszSection != NULL);

	HKEY hSectionKey = NULL;
	HKEY hAppKey = GetAppRegistryKey();
	if (hAppKey == NULL)
		return NULL;

	DWORD dw;
	RegCreateKeyEx(hAppKey, lpszSection, 0, REG_NONE,
		REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
		&hSectionKey, &dw);
	RegCloseKey(hAppKey);
	return hSectionKey;
}

void Application::SetKeyName(const char* company, const char* profile)
{
	pszRegistryKey	= sdup(company);
	pszProfileName	= sdup(profile);
}

HKEY Application::GetAppRegistryKey()
{
	ASSERT(pszRegistryKey != NULL);
	ASSERT(pszProfileName != NULL);

	HKEY hAppKey = NULL;
	HKEY hSoftKey = NULL;
	HKEY hCompanyKey = NULL;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, szSoftware, 0, KEY_WRITE|KEY_READ,
		&hSoftKey) == ERROR_SUCCESS)
	{
		DWORD dw;
		if (RegCreateKeyEx(hSoftKey, pszRegistryKey, 0, REG_NONE,
			REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
			&hCompanyKey, &dw) == ERROR_SUCCESS)
		{
			RegCreateKeyEx(hCompanyKey, pszProfileName, 0, REG_NONE,
				REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
				&hAppKey, &dw);
		}
	}
	if (hSoftKey != NULL)
		RegCloseKey(hSoftKey);
	if (hCompanyKey != NULL)
		RegCloseKey(hCompanyKey);

	return hAppKey;
}
