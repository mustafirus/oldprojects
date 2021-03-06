#pragma once
#include <CRTDBG.H>

#ifdef _DEBUG

#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)

#define DBG_INIT \
	_CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_WNDW);\
	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_WNDW);\
	_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_DEBUG);\
	int tmpFlag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG );\
	tmpFlag |= _CRTDBG_CHECK_ALWAYS_DF|_CRTDBG_LEAK_CHECK_DF;\
	_CrtSetDbgFlag( tmpFlag );

#define VERIFY(X) _ASSERTE(X)
#define ASSERT(x) _ASSERTE(x)

#else

#define DBG_INIT
#define VERIFY(X) X
#define ASSERT(X)

#endif //_DEBUG

#define TRACE1(msg, arg1)  _RPT1(_CRT_WARN, msg, arg1)
#define TRACE2(msg, arg1, arg2)  _RPT2(_CRT_WARN, msg, arg1, arg2)
