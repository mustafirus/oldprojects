#pragma once
//void cfgsave();
//void log(char* s1);
#define APPNAME "MyProxy"
#define MAXDEFPROPSIZE 1024
#define MAXPKGS 64
#define MAXPKGLEN 64

#ifdef CONFIG_DEFINED

const char* svcname = APPNAME;
extern char svcdir[_MAX_PATH] = "";

char srcdir[_MAX_PATH] = "";
char defprops[MAXDEFPROPSIZE] = "";

char pkgs[MAXPKGS][MAXPKGLEN];
int	 pkg_cnt=0;

#else

extern const char* svcname;
extern char svcdir[_MAX_PATH];

extern char srcdir[_MAX_PATH];
extern char defprops[MAXDEFPROPSIZE];
extern char pkgs[MAXPKGS][MAXPKGLEN];
extern int	pkg_cnt;

#endif

bool cfginit();
void redirect_stdout();
void set_confdir(const char* optarg);
char* catpath(const char* dir, const char* file, char* ext);
void rtrim(char* str, int len = 0);
