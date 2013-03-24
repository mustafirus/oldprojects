// installer.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "installer.h"
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

Plugin* GetPlugin(XmlRpcServer* s, Config* cfg){
	return new Installer(s, cfg);
}
