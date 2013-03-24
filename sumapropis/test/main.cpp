#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
char* SumaPropis(unsigned long iNum,char* sentens);

main( int argc, char *argv[ ], char *envp[ ] )
{
	unsigned long l;
	char buffer[1024];
	do{
		gets(buffer);
		l=atol(buffer);
		SumaPropis(l,buffer);
		AnsiToOem(buffer,buffer);
		puts(buffer);
	}while(l!=0);
	return 0;
}