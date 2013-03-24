// compiler.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "devframe.h"
#include "guid.h"
#include "getopt.h"

void usage();

int yyparse (void);

int _tmain(int argc, _TCHAR* argv[])
{
	const char* infile = NULL;
	const char* outfile = NULL;
	const char* optstring = "hi:o:";
	int	c = getopt (argc, argv, optstring);
	while(c > 0){
		switch(c) {
		case 'i':
//			infile = strdup(optarg);
			infile = optarg;
			break;
		case 'o':
			outfile = optarg;
			break;
		case '?':
		case 'h':
		default:
			usage();
			return -1;
		}
		c = getopt (argc, argv, optstring);
	}
	extern FILE *yyin, *yyout;
	FILE *out;
	if ( !infile || !outfile )
		return usage, -1;
	yyin = fopen( infile, "r" );
	out = fopen( outfile, "w" );
	//yyin = stdin;
	yyparse();
	fclose(yyin);


	int num = DevFrame::_root->size();
	FrameData* buf = new FrameData[num];
	DevFrame::_root->dump(buf);
//	delete DevFrame::_root;
//	DevFrame::_root = new DevFrame(buf, 0);
	char* s1 = new char[1024];
	DevFrame::_root->dump(s1);

	fwrite( buf, sizeof(FrameData), num, out );
	fclose(out);

	
	return 0;
}

void usage()
{
	printf("Usage:"
		"compiler -i <infile> -o <outfile>\n"
		"compiler [ -h | -? ]\n");
}
