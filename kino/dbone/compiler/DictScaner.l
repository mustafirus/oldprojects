%{
#include "stdafx.h"

typedef string YYSTYPE;
#define YYSTYPE_IS_DECLARED 1
#include "DictParser.hpp"
%}
%option never-interactive
%option noyywrap
%x PARAMS
%x CODE

ID		[a-zA-Z][a-zA-Z0-9]*
VNAME	{ID}("."{ID})*
CHAR	[a-zA-Z]

%%

"{"				yylval = ""; return '{';
"}"				yylval = ""; return '}';
","				yylval = ""; return ',';
"."				yylval = ""; return '.';
";"				yylval = ""; return ';';
":"				yylval = ""; return ';';
"<"				yylval = ""; return '<';
">"				yylval = ""; return '>';
"domain"		yylval = ""; return domain;
"table"			yylval = ""; return table;
"view"			yylval = ""; return view;
{ID}			yylval = yytext; return NAME;
{VNAME}			yylval = yytext; return VNAME;

"["				yylval = ""; BEGIN(PARAMS);		return '[';
<PARAMS>"]"		yylval = ""; BEGIN(INITIAL);	return ']';
<PARAMS>","		yylval = ""; return ',';
<PARAMS>{ID}	yylval = yytext; return NAME;
<PARAMS>"=>"	yylval = ""; return op_ptr;
<PARAMS>[.*]	yylval = yytext; return VALUE;

<CODE>"}"		yylval = ""; BEGIN(INITIAL); return '}';
<CODE>".*"		yylval = yytext; return BODY;

%%

void begin_code(){
	BEGIN(CODE);
}