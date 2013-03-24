#pragma once
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#define ifdel(x) if(x) delete x;
inline char* sdup(const char* str, unsigned int len)
{	
	int l = min(len, strlen(str));
	char* s1 = new char[l+1];
	strncpy(s1, str, l);
	return s1;
}
inline char* sdup(const char* str)
{
	if(str && *str)
	{
		char* s1 = new char[strlen(str)+1];
		strcpy(s1, str);
		return s1;
	}
	return NULL;
}
#define iterate(X, I) for(##I = 0; ##I < ##X.GetCount(); i++)
