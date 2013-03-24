#include "Time.h"
#include "Log.h"
class Clock
{
public:
	clock_t clk_start;
	Log* pLog;
	const char* str;
	Clock(const char* name, Log* pl) :
	str(name), pLog(pl) {clk_start = clock();};
	~Clock()
	{
		double dif = (double)(clock() - clk_start)
			/ CLOCKS_PER_SEC;
		pLog->printf("%s : %f sec\n", str, dif);
	};
};