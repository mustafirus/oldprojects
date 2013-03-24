#include "stdafx.h"
#include "service.h"
#include "config.h"
#include "Socket.h"
#include "Installer.h"

#define PORT 514

sockaddr_in def_addr = {AF_INET, PORT, INADDR_ANY};
//sockaddr_in def_addr = {AF_INET, IPPORT_CMDSERVER, INADDR_ANY};
HANDLE hExit = NULL;

void service_shutdown()
{
//	CloseHandle(hTimer);

//	WSACleanup();
	SetEvent(hExit);
}
#define TIME_01SEC 1000
#define TIME_15MIN 1000 * 60 * 15
void service_startup()
{
//	hTimer = CreateWaitableTimer( NULL, FALSE, NULL );
//	BOOL b = SetWaitableTimer( hTimer, NULL, TIME_01SEC, NULL, NULL, FALSE);

//	WSAData data;
//	WSAStartup ( MAKEWORD( 2, 2 ),  &data);

	hExit = CreateEvent(NULL, TRUE, FALSE, NULL); // exit event 
}
void service_cleanup()
{
	CloseHandle(hExit);
}

#define PROPSLEN 1024

#if 0
DWORD WINAPI session( SOCKET* pcon ){

	Socket sock(*pcon);
	Installer inst(&sock);
	while(!!sock)
	{
		char cmd[80];
		*cmd = 0;
		sock.getline(cmd, 80);
		switch(*cmd)
		{
/*		case 'g':
			{
				char fn[_MAX_PATH];
				sprintf(fn, "%s%s", svcpath, cfgname);
				char* cfg=cmd+1;
				cfg += strspn(cfg, " \t");
				BOOL ret = CopyFile(cfg, fn, FALSE);
				if(ret) {
					sock.send("Ok\n");
				}else {
					DWORD err = GetLastError();
					char e[20];
					sprintf(e, "...error %x\n", err);
					sock.send(e);
				}

			}
			break;*/
		case 'i':
			inst.run();
			break;
		case 'q':
			return 0;
		case '?':
		case 'h':
			sock.send(
//				"g <cfgfileUNC> - get config file\n"
				"i - installer module\n"
				"q - quit\n"
				"h | ? - help\n"
				);
			break;
		default:
			sock.send("Unknown command. Press h | ? for help\n");
		}
	}
	return 0;
}
#endif// 0
#if 0
void check_msi_ret(UINT ret) {
	if(ret == ERROR_SUCCESS) {
		printf("Ok\n");
	}else {
		printf("...error 0x%x\n", ret);
	}
}
#endif// 0

enum msi_state { msi_error, installed, need_upgrade, not_installed };

#define CODE_SIZE 64
msi_state check_state(const char* pkgname){
	UINT ret;
	char code[CODE_SIZE];
	char old_pkg_code[CODE_SIZE];
	char new_pkg_code[CODE_SIZE];
	DWORD code_size;
	MSIHANDLE hPkg = NULL;
	MSIHANDLE hInstalled = NULL;

	char fullpkgname[_MAX_PATH];
	strcpy(fullpkgname, catpath(srcdir, pkgname, ".msi"));

	ret = MsiGetSummaryInformation( NULL, fullpkgname, 0, &hPkg);
	if(ret != ERROR_SUCCESS) return msi_error;
	UINT uiDataType;
	INT iValue;
	FILETIME ftValue;

	ret = MsiSummaryInfoGetProperty( hPkg, PID_REVNUMBER,
		&uiDataType, &iValue, &ftValue, old_pkg_code, &code_size );
	
/*	ret = MsiOpenPackage( fullpkgname, &hPkg );
	if(ret != ERROR_SUCCESS) return msi_error;
	
	code_size = CODE_SIZE;
	ret = MsiGetProductProperty( hPkg,
		"ProductCode", code, &code_size);
	if(ret != ERROR_SUCCESS) return msi_error;

	code_size = CODE_SIZE;
	ret = MsiGetProductProperty( hPkg, //INSTALLPROPERTY_PACKAGECODE
		"PackageCode", new_pkg_code, &code_size);
	if(ret != ERROR_SUCCESS) return msi_error;
	
	ret = MsiCloseHandle( hPkg );
*/
	switch (MsiQueryProductState( code )){
		//		case INSTALLSTATE_ADVERTISED:
		//The product is advertised but not installed. 
		//		case INSTALLSTATE_BAD_CONFIGURATION:
		//The configuration data is corrupt. 
	case INSTALLSTATE_DEFAULT:	//The product is installed for the current user. 
		
		code_size = CODE_SIZE;
		ret = MsiGetProductInfo( code,
			"PackageCode", old_pkg_code, &code_size);
		if(ret != ERROR_SUCCESS) return msi_error;

		return strcmp(old_pkg_code, new_pkg_code) ? need_upgrade : installed;
	
	case INSTALLSTATE_UNKNOWN:	//The product is neither advertised or installed. 
	case INSTALLSTATE_ABSENT:	//The product is installed for a different user. 
	
		return not_installed;
	
	case INSTALLSTATE_INVALIDARG:	//An invalid parameter was passed to the function.
		printf("An invalid parameter (%s) was passed to the function(MsiQueryProductState).");
		
		return msi_error;
	
	}
	return msi_error;
}

void msi_install_upgrade(const char* pkgname, bool install){
	char fullpkgname[_MAX_PATH];
	strcpy(fullpkgname, catpath(srcdir, pkgname, ".msi"));
	char pkgprops[_MAX_PATH];
	strcpy(pkgprops, catpath(srcdir, pkgname, ".prp"));

	char props[MAXDEFPROPSIZE * 2];

	strcpy(props, defprops);
	FILE* f = fopen(pkgprops, "r");
	if(f) {
		char line[128];
		while(fgets(line, 128, f)) {
			rtrim(line);
			strcat(props, " ");
			strcat(props, line);
		}
	}

	printf("installing %s %s\n", fullpkgname, props);
	UINT ret = MsiInstallProduct( fullpkgname, props );
	if(ret != ERROR_SUCCESS){
		printf("--!! error code %d\n", ret);
	};
}

void msi_install(const char* pkgname){
	msi_install_upgrade(pkgname, true);
}
void msi_upgrade(const char* pkgname){
	msi_install_upgrade(pkgname, false);
}

void run(bool debug = false){
	service_startup();
	while(WAIT_TIMEOUT == WaitForSingleObject( hExit, TIME_01SEC)){
		if(!cfginit())
			continue;
		for(int i = 0; i < pkg_cnt; i++){
			switch(check_state(pkgs[i])){
			case installed:
				break;
			case need_upgrade:
				msi_upgrade(pkgs[i]);
				break;
			case not_installed:
				msi_install(pkgs[i]);
				break;
			}
		}
	}
	service_cleanup();
}
#if 0
void run_bak(bool debug = false)
{
	service_startup();

	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if ( s == INVALID_SOCKET )
	{
		printf("Cant create socket");
	}
	if (bind ( s, (sockaddr*) &def_addr, sizeof(def_addr)) == SOCKET_ERROR)
	{
		printf("Cant bind socket");
		return;
	}

	for(;;)
	{
		int ret = listen(s, SOMAXCONN);
		if(ret == SOCKET_ERROR )
		{
			return;
		};
		SOCKET		con;
		sockaddr	conaddr;
		int			conaddrlen = sizeof(conaddr);
		con = accept ( s, &conaddr, &conaddrlen);
		if ( con == INVALID_SOCKET )
		{
			int err = WSAGetLastError();
			continue;
		}
		DWORD ThreadId;
		HANDLE hThread = CreateThread( NULL, 0, 
			(LPTHREAD_START_ROUTINE)session, &con, 0, &ThreadId );
	}
}
#endif //0 

