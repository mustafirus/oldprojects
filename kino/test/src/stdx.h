//#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
//#include <commctrl.h>
#include <malloc.h>
#include <string.h>
#include <limits.h>
//#include <sql.h>
//#include <sqlext.h>
#include "hlpdefs.h"
//#include "x_dbg.h"
//#define new	DEBUG_NEW
#define ASSERT(X)
typedef unsigned long DWORD;
typedef unsigned int UINT;
typedef unsigned int HWND;
struct tag_SZ {
	int cx;
	int cy;
};
typedef tag_SZ SIZE;
#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))
#define _MAX_PATH	1024
