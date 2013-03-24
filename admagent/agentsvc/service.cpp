// evl2db.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "service.h"

Service::ClList	Service::cl_list[SERVICE_MAX];
int				Service::cl_num = 0;

Service::Service(const char* name){
	if(cl_num >= SERVICE_MAX){
		throw ServiceException();

	}
	hExit = CreateEvent(NULL, TRUE, FALSE, NULL); // exit event 
	ss.dwServiceType = cl_num == 1 ? SERVICE_WIN32_OWN_PROCESS : SERVICE_WIN32_SHARE_PROCESS;
	ss.dwCurrentState = SERVICE_START_PENDING;
	ss.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	ss.dwWin32ExitCode = 0;
	ss.dwServiceSpecificExitCode = 0;
	ss.dwCheckPoint = 0;
	ss.dwWaitHint = 0;
	cl_list[cl_num].name = (char*)name;
	cl_list[cl_num++].cl = this;
}

void Service::run_servers(){
//	__asm int 3
	if (!cl_num) return;
	SERVICE_TABLE_ENTRY* pd = new SERVICE_TABLE_ENTRY[cl_num+1];
	for(int i = 0; i < cl_num; i++){
		pd[i].lpServiceName = cl_list[i].name;
		pd[i].lpServiceProc = ServiceMain;
	};
	pd[i].lpServiceName = NULL;
	pd[i].lpServiceProc = NULL;
	StartServiceCtrlDispatcher(pd);
};
void Service::shutdown_servers(){
	for(int i = 0; i < cl_num; i++){
		cl_list[i].cl->shutdown();
	};
};

void WINAPI Service::ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv)
{
//	__asm int 3;
	for(int i = 0; i < cl_num; i++){
		if (!strcmp( cl_list[i].name, *lpszArgv)) break;
	};
	if( i == cl_num) throw ServiceException();
	Service* ps = cl_list[i].cl;


	ps->hss = RegisterServiceCtrlHandlerEx(	cl_list[i].name, HandlerEx, ps );
	if ( !ps->hss ) return;

	ps->ss.dwCurrentState = SERVICE_RUNNING; 
	if (!SetServiceStatus (ps->hss, &ps->ss)) return;

	SERVICE_STATUS_HANDLE hss = ps->hss;
	SERVICE_STATUS ss = ps->ss;
	ps->run();

//	__asm int 3;
	ss.dwCurrentState  = SERVICE_STOPPED; 
	SetServiceStatus(hss,  &ss);
}

DWORD WINAPI Service::HandlerEx( DWORD dwControl, DWORD dwEventType, LPVOID lpEventData, LPVOID lpContext){
	Service* ps = static_cast<Service*>(lpContext);

	SERVICE_STATUS_HANDLE hss = ps->hss;
	SERVICE_STATUS ss = ps->ss;

	switch(dwControl)
	{
	case SERVICE_CONTROL_PAUSE:
		ps->ss.dwCurrentState = SERVICE_RUNNING;
		ss = ps->ss;
		break; 
	case SERVICE_CONTROL_CONTINUE: 
		ps->ss.dwCurrentState = SERVICE_RUNNING; 
		ss = ps->ss;
		break;
	case SERVICE_CONTROL_STOP: 
		ps->ss.dwCurrentState  = SERVICE_STOP_PENDING; 
		ss = ps->ss;
//		__asm int 3
		ps->shutdown();

		break;
	case SERVICE_CONTROL_INTERROGATE: 
		// Fall through to send current status.
		break;  
	default:
		// Urecognized otption
		return ERROR_CALL_NOT_IMPLEMENTED;
	}
	SetServiceStatus (hss,  &ss);
	return NO_ERROR;
} 

