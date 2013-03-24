// evl2db.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "getopt.h"
#include "string.h"
#include "config.h"
#include "service.h"

//char filepath[_MAX_PATH];

int usage(bool help = false);
void WINAPI Handler (DWORD Opcode);
void WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv);
BOOL WINAPI CBreak(DWORD dwCtrlType);
void run(bool debug = false);

void service_shutdown_int()
{
	service_shutdown();
};
int __cdecl main(int argc, char* argv[])
{
	enum {none, daemon, console} runmode = none;
//	bool daemon = false;
	if(argc == 1)
		return usage(true);
	const char* optstring = "d$?c::";
	int	c = getopt (argc, argv, optstring);
	while(c > 0){
		switch(c) {
		case 'c':
			set_confdir(optarg);
			break;
/*		case 'i':
			{
				SC_HANDLE hSC = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
				if(!hSC)
					return printf("Error: Can't create service %s.\n", svcname), -1;
				char path[_MAX_PATH];
				sprintf(path, "%s -$", filepath);
				SC_HANDLE hSvc = CreateService(hSC, svcname, svcname, 0,
					SERVICE_WIN32_OWN_PROCESS, SERVICE_AUTO_START, SERVICE_ERROR_IGNORE,
					path, NULL, NULL, DEPENDS, NULL, NULL);
				if(hSvc)
				{
					CloseServiceHandle(hSvc);
					printf("Service '%s' succesfuly created.\n", svcname);
				}
				else 
					printf("Error: Can't create service '%s'.\n", svcname), -1;
				CloseServiceHandle(hSC);
			}
			break;
		case 'r':
			{
				SC_HANDLE hSC = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
				if(!hSC)
					return printf("Error: Can't delete service '%s'.\n", svcname), -1;
				SC_HANDLE hSvc = OpenService(hSC, svcname, DELETE);
				if(DeleteService(hSvc))
					printf("Service '%s' succesfuly deleted.\n", svcname);
				else
					printf("Error: Can't delete service %s.\n", svcname);
				CloseServiceHandle(hSvc);
			}
			break;*/
		case 'd':
			runmode = console;
			break;
		case '?':
			return usage(true);
		case '$':
			runmode = daemon;
			break;
		default:
			return usage();
		}
		c = getopt (argc, argv, optstring);
	}
	switch(runmode)
	{
	case console:
		{
			//		logtime();
			printf("Debug run.\n");
//			cfginit();
			SetConsoleCtrlHandler(CBreak, TRUE);
			run(true);
			//		logtime();
			printf("Exit debug run.\n");
		}
		break;
	case daemon:
		{
			redirect_stdout();
			SERVICE_TABLE_ENTRY   DispatchTable[] =
			{
				{(char*)svcname, ServiceMain },
				{NULL, NULL}
			};  
			StartServiceCtrlDispatcher(DispatchTable);
		}
		break;
	case none:
		return -1;
	}

	_flushall();
	return 0;
}

int usage(bool help)
{
	if(help)
	printf("Usage:"\
"MyProxy {-$ | -d | -?} {-c<confdir>}\n"\
"Where:\n"\
"	-d debug run\n"\
"	-? - help\n");
	else
		printf("Syntax error!\n MyProxy -? for help.");

	_flushall();
	return help ? 0 :-1;
}


SERVICE_STATUS_HANDLE hss;
SERVICE_STATUS ss = {SERVICE_WIN32_OWN_PROCESS, SERVICE_START_PENDING,
SERVICE_ACCEPT_STOP/* | SERVICE_ACCEPT_PAUSE_CONTINUE*/, 0, 0, 0, 0};

void WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv)
{
	hss = RegisterServiceCtrlHandler(svcname, Handler);  
	if (hss == (SERVICE_STATUS_HANDLE)0)
	{
		//		SvcDebugOut(" [MY_SERVICE] RegisterServiceCtrlHandler failed %d\n", GetLastError());         
		return;
	}  
	// Initialization code goes here. 
	// Handle error condition
//	logtime();
	printf("Service starting.\n");
#if 0
	if (!cfginit())
	{ 
		ss.dwCurrentState = SERVICE_STOPPED; 
		ss.dwWin32ExitCode 	 = -1; 
		SetServiceStatus (hss, &ss); 
//		logtime();
		printf("Service stoped\n");
		return;
	}
#endif
	// Initialization complete - report running status. 
	ss.dwCurrentState = SERVICE_RUNNING; 
	if (!SetServiceStatus (hss, &ss))
	{ 
		return;
	}

//////////////////////////////////////////
//	logtime();
	printf("Service started.\n");
	run();
//	logtime();
	printf("Service stoped\n");
//////////////////////////////////////////
	ss.dwCurrentState  = SERVICE_STOPPED; 
	SetServiceStatus(hss,  &ss);
}

void WINAPI Handler (DWORD Opcode)
{
    switch(Opcode)
	{
	case SERVICE_CONTROL_PAUSE:
		ss.dwCurrentState = SERVICE_PAUSED;
		break; 
	case SERVICE_CONTROL_CONTINUE: 
		ss.dwCurrentState = SERVICE_RUNNING; 
		break;
	case SERVICE_CONTROL_STOP: 

		service_shutdown_int();
        // Do whatever it takes to stop here. 
		ss.dwCurrentState  = SERVICE_STOP_PENDING; 
		break;
	case SERVICE_CONTROL_INTERROGATE: 
		// Fall through to send current status.
		break;  
	default:
		// Urecognized otption
		break;
	}
	SetServiceStatus (hss,  &ss);
} 
BOOL WINAPI CBreak(DWORD dwCtrlType)
{
	service_shutdown_int();
	return TRUE;
}

void logtime()
{
	time_t t;
	time(&t);
	tm* ptm = localtime(&t);
	printf("%d/%02d/%02d %02d:%02d:%02d\t", ptm->tm_year + 1900, ptm->tm_mon+1, ptm->tm_mday,
		ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
}

