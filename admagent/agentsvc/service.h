#pragma once
#define SERVICE_MAX	8
class ServiceException{};

class Service{
	SERVICE_STATUS_HANDLE hss;
	SERVICE_STATUS ss;
	HANDLE hExit;
protected:
	Service(const char* name);
	~Service(){
		CloseHandle(hExit);
	};
	virtual void run(){
		ResetEvent(hExit);
	};
	virtual void shutdown(){
		SetEvent(hExit);
	};
	virtual bool must_exit(){
		return WaitForSingleObject(hExit,0)==WAIT_OBJECT_0;
	};


//Static members
public:
	static void run_servers();
	static void shutdown_servers();


private:
	static void WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv);
	static DWORD WINAPI HandlerEx( DWORD dwControl, DWORD dwEventType, LPVOID lpEventData, LPVOID lpContext);

	struct ClList{
		char*		name;
		Service*	cl;
	};
	static ClList		cl_list[SERVICE_MAX];
	static int			cl_num;
};
