//#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "verbs.h"

#define NUMDIGITS 64

char* SumaPropis(unsigned long iNum,char* sentens)
{
	int i=0,max=0;
	int c=0,r=0;
	bool triplet=false;
	char digits[NUMDIGITS];
	char* s1=sentens;
	_ultoa(iNum,digits,10);
	max=strlen(digits)-1;
	strrev(digits);
	for(i=max;i>=0;i--)digits[i]-=48;
	for(i=1;i<=max;i+=3)
		if(digits[i]==1)
		{
			digits[i]=0;
			digits[i-1]+=10;
		};
	if(digits[max]==0)max--;
	for(i=max;i>=0;i--)
	{
		if(digits[i]==0)goto lScip;
		triplet=true;
		r=0;
		switch(i%3)
		{
		case 0:
			c=1;
			r=(i/3-1)*3;
			if(digits[i]==1)r+=1;
			else if(digits[i]>1 && digits[i]<5)r+=2;
			else r+=3;
			break;
		case 1:c=19;break;
		case 2:c=28;
		};
		c+=digits[i];
		sprintf(s1,"%s ",cverb[c]);
		s1+=strlen(s1);
lScip:
		if(triplet && r>0)
		{
			triplet=false;
			sprintf(s1,"%s ",cverb[37+r]);
			s1+=strlen(s1);
		};
	};
	return sentens;
};