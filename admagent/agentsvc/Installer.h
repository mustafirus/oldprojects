#pragma once
#include "plugin.h"

#ifdef PLUGINNAME
#undef PLUGINNAME
#endif

#define PLUGINNAME "installer"


class Installer : public Plugin
{
#if 0
	Socket* sock;
	char* cmd;
	bool getprod(char* cmd, char* prod, char* props, char* code = NULL, DWORD code_size = 0 );
	void check_msi_ret(UINT ret);
public:
	Installer(Socket* s) : sock(s){cmd = new char[cmdsz];*cmd=0;};
	virtual ~Installer(){delete[] cmd;};
	void run();
//	bool parsebuf(char* buf, int* datalen);
static const int cmdsz;
#endif


	string	list();
	void	remove(string code);
	void	install(string path, string props);
	void list_rpc(XmlRpcValue& params, XmlRpcValue& result);
//	DECMETHOD0_1(list);
	DECMETHOD1_0(remove);
	DECMETHOD2_0(install);
public:
	Installer(XmlRpcServer* s, Config* cfg);
	virtual ~Installer(){};
};
