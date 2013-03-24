%{
#include "stdafx.h"

typedef string YYSTYPE;
#define YYSTYPE_IS_DECLARED 1
#include "FrameParser.hpp"
%}
%option never-interactive
%option noyywrap

ID		[a-zA-Z][a-zA-Z0-9]*
FIELD	{ID}("."{ID})*
CHAR	[a-zA-Z]

%%

"<"			yylval = ""; return '<';
">"			yylval = ""; return '>';
","			yylval = ""; return ',';
{CHAR}		yylval = yytext; return FRAME_TYPE;
{FIELD}		yylval = yytext; return FIELD_NAME;

%%
int a = 1;

