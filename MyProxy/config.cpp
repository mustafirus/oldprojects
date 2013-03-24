#include "stdafx.h"

#define CONFIG_DEFINED
#include "config.h"

	  char  confdir[_MAX_PATH] = "";
const char* confnam = APPNAME".conf";

const char* logname = APPNAME".log";
const char* rulfile = APPNAME".rul";

char* SEP = " =,\n";


char* catpath(const char* dir, const char* file, char* ext = NULL)
{
	static char fullname[_MAX_PATH] = "";
	strcpy(fullname, dir);
	char* s1 = fullname + strlen(fullname);
	assert( *s1 != ' ');
	if( *(s1-1) != '\\'){
		*s1++ = '\\';
		*s1 = 0;
	}
	strcpy(s1, file);
	if(ext != NULL)
		strcat(s1, ext);
	return fullname;
}

void redirect_stdout(){
	freopen(catpath(svcdir, logname), "a", stdout);
}

void rtrim(char* str, int len)
{
	char* s1 = str + (len ? len : strlen(str));
	while(s1-- > str)
	{
		if(*s1 == ' ' || *s1 == '\t' || *s1 == '\n' || *s1 == '\r')
			*s1 = 0;
		else
			break;
	}
//	return s1+2;
}
void set_confdir(const char* optarg){
	if(!optarg)
		return;
	strcpy(confdir, optarg);
	rtrim(confdir);
}

int initvar_i(char* str, char* varname, int* var)
{
	int varlen = strlen(varname);
	if(!strnicmp(str, varname, varlen))
	{
		char* s1=strtok(str+varlen, SEP);
		if(s1)
			*var = atoi(s1);
		return 1;
	}
	return 0;
}

int initvar_s(char* str, char* varname, char* var)
{
	int varlen = strlen(varname);
	if(!strnicmp(str, varname, varlen))
	{
		char* s1=strchr(str+varlen, '=');
		if(s1)
			s1 += strspn(s1, SEP);
			strcpy(var, s1);
		return 1;
	}
	return 0;
}

void cfgsave()
{
//	saveprops();
}

void add_pkg(const char* pkg)
{
	if(pkg_cnt >= MAXPKGS)
	{
		printf("Pkg count reached limit %s\n");
		return;
	}
	strncpy(pkgs[pkg_cnt], pkg, MAXPKGLEN - 1);
	pkgs[pkg_cnt++][MAXPKGLEN] = 0;
}

bool getrul(){
	FILE* f = fopen(catpath(confdir, rulfile), "r");
	if(!f){
		printf("Rul file '%s' not found\n", catpath(confdir, rulfile));
		fclose(f);
		return false;
	}
	char str[1024];
	int cursrc = -1;
	int line = 0;
	while(fgets(str, 1024, f))
	{
		line++;
		rtrim(str);
		switch(*str)
		{
		case '#':break;
		case 0: break;
		default:
			{
				char* pkg = strtok(str, "\t ");
				char* comps = strtok(NULL, "\n\r");
				if(!comps)
					break;
				char comp[_MAX_PATH];
				DWORD comp_len = _MAX_PATH - 1;
				if(!GetComputerName( comp, &comp_len ))
					break;
				
				if(strstr(_strupr( comps ), _strupr( comp )))
					add_pkg(pkg);
			}
			
		}
	}
	fclose(f);
	return true;
}

bool cfginit()
{
	GetModuleFileName(NULL, svcdir, _MAX_PATH);
	char* ch = strrchr(svcdir, '\\');
	if(!ch)
		return false;
	*ch = 0;

	if(! *confdir){
		return false;
	}

	FILE* f = fopen(catpath(confdir, confnam), "r");
	if(!f)
	{
		printf("Warning CFG: Configuration file '%s' not found\n", catpath(confdir, confnam));
		return true;
	}
	char str[1024];
	int cursrc = -1;
	int line = 0;
	while(fgets(str, 1024, f))
	{
		line++;
		rtrim(str);
		switch(*str)
		{
		case '#':break;
		case 0: break;
		default:
			{
				int varok = 0;
				varok |= initvar_s(str, "defprops", defprops);
				varok |= initvar_s(str, "srcdir", srcdir);
				varok = 1;
				if(!varok)
				{
					printf("Warning CFG: Unknown option in configuration file, line %d\n", line);
				}
			}
			
		}
	}
	
	fclose(f);
	return getrul();
}
