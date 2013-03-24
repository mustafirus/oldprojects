// agentsvc.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "AdmSvc.h"
#include "Hello.h"
#include "Registry.h"

string basepath(const char* path){
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT ];
	_splitpath(path, drive, dir, fname, ext);
	string s;
	return s + drive + dir;
}
int _tmain(int argc, _TCHAR* argv[])
{
	Registry::Value::initmaps();
	AdmSvc s(basepath(argv[0]));
	if( argc > 1 && *argv[1] == '$'){
		s.run();
	}else{
		Service::run_servers();
	}
	return 0;
}
//Ws2_32.lib msi.lib Netapi32.lib
/*
void ttt(){
	Registry reg(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall");
	Registry::Keys keys;
	reg.enum_subkeys(keys);
	for ( Registry::Iter it = keys.begin(); it != keys.end( ); it++ ){
		Registry r(reg, *it);
		r.flags |= RF_NOT_THROW_NOT_FOUND;
		string name, prodid;
		
		name = r.get("DisplayName");
		prodid = r.get("ProductID");
		cout << (string)r << "|" << name << "|" << prodid;
		printf("%s:%s:%s\n", (*it).c_str(), name.c_str(), prodid.c_str());
	}
}
*/
