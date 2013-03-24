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

#else

#define DBG_INIT
#define VERIFY(X) X

#endif //_DEBUG
#define ASSERT(x) _ASSERTE(x)

#define TRACE1(msg, arg1)  _RPT1(_CRT_WARN, msg, arg1)
