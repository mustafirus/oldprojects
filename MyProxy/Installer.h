// Instaler.h: interface for the Instaler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INSTALER_H__02D275AD_6D21_4953_A523_A585585EAF4C__INCLUDED_)
#define AFX_INSTALER_H__02D275AD_6D21_4953_A523_A585585EAF4C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Installer  
{
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
};

#endif // !defined(AFX_INSTALER_H__02D275AD_6D21_4953_A523_A585585EAF4C__INCLUDED_)
