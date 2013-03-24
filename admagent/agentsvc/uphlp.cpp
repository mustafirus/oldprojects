#include <windows.h>
#include <tchar.h>
#include <string>
#include <io.h>
#include <direct.h>

using namespace std;
class str : public string{
public:
	str(){};
	str(const char* s1): string(s1){};
	str(string& s1): string(s1){};
	operator const char* (){return c_str();}
};
str operator+( const str& _Left, const char* _Right)
{return ( str(_Left) += _Right);};
str operator+( const str& _Left, const str& _Right)
{return ( str(_Left) += _Right);};

str basepath(const char* path){
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT ];
	_splitpath(path, drive, dir, fname, ext);
	return str() + drive + dir;
}

str basename(const char* path){
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT ];
	_splitpath(path, drive, dir, fname, ext);
	return str() + fname + ext;
}

void trim(str& s){
	for(str::iterator it = s.end()-1; it != s.begin(); it--){
		char c = *it;
		if(c == ' ')
			continue;
		if(++it == s.end())
			break;
		s.erase(it,s.end());
		break;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	SC_HANDLE scm = OpenSCManager(NULL, NULL,SC_MANAGER_ALL_ACCESS);
	if(!scm)
		return -1;
	SC_HANDLE sc = OpenService( scm, "AdmAgent", SERVICE_ALL_ACCESS);
	if(!sc)
		return -1;
	SERVICE_STATUS ss;
	memset(&ss,0,sizeof(ss));
	if(!QueryServiceStatus( sc, &ss ))
		return -1;
	bool autostart = false;
	switch(ss.dwCurrentState){
	case SERVICE_RUNNING:
		autostart = true;
		ControlService( sc, SERVICE_CONTROL_STOP,&ss);
		break;
	case SERVICE_STOPPED:
		break;
	default:
		return -1;
	}
	for( int n = 0; n < 10; n++){
		switch(ss.dwCurrentState){
		case SERVICE_STOP_PENDING:
			Sleep(1000);
			QueryServiceStatus( sc, &ss );
			continue;
		case SERVICE_STOPPED:n=10;
			break;
		default:
			return -1;
		}
	}

	str bp = basepath(argv[0]);
	str up;
	if(argc == 1){
		up = bp+"update\\";
	}else{
		up = argv[1];
		trim(up);
		if(up[up.length()-1] != '\\'){
			up+="\\";
		}
	}
	_finddata_t fileinfo;
	intptr_t f = _findfirst( up+"*", &fileinfo );
	while(f != -1){
		if(*fileinfo.name != '.'){
			remove(bp + fileinfo.name);
			rename(up+fileinfo.name, bp + fileinfo.name);
		}
		if(_findnext( f, &fileinfo )==-1){
			_findclose( f );
			f = -1;
		}
	}
	if (autostart && StartService( sc, 0, NULL))
		return 0;
	return -1;
}
