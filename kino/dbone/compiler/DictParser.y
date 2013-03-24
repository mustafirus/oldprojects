%{
#include "stdafx.h"

typedef string YYSTYPE;
#define YYSTYPE_IS_DECLARED 1
#include "DictParser.hpp"

int yylex ();
void yyerror (char const *s);
void begin_code();

%}
%token domain table view op_ptr NAME VNAME VALUE TYPE ARG BODY
%start stmts
%defines

/* -------------- rules section -------------- */

%%

stmts 	: stmt
		| stmts stmt
		;
stmt 	: tab_def
		| dom_def
		| view_def
		;
dom_def			: domain NAME '[' {/*begin_add_domain()*/} params ']' ';'
				| domain NAME ':' NAME '[' {/*begin_add_domain()*/} params ']' ';'
				;
tab_def			: table NAME '{' tab_components '}'  ';'
				| table NAME ':' NAME '{' tab_components '}'  ';'
				;
view_def		: view NAME '{' {/*begin_add_view()*/} view_components '}'  ';'
					{ /*end_add_view(); */}
				| view NAME ':' NAME '{' {/*begin_add_view()*/} view_components '}'  ';'
					{ /*end_add_view(); */}
				;
			
tab_components	: tab_component
				| tab_components tab_component
				;
view_components	: view_component
				| view_components view_component
				;
tab_component	: field
				| reference
				| function
				| method
				;
view_component	: vfield
				| function
				| method
				;

field_hdr		: NAME NAME
				| NAME NAME '[' params ']'
				;
vfield_hdr		: VNAME
				| VNAME '[' params ']'
				;

field 			: field_hdr ';'
				;
vfield 			: vfield_hdr ';'
				;
reference		: field_hdr op_ptr NAME ';'
				;
function		: field_hdr '(' ')' '{' { begin_code(); } BODY '}' ';'
				;
method			: NAME '(' ')' '{' { begin_code(); } BODY '}' ';'
				;

params			: param
				| params ',' param
				;
param			: NAME op_ptr VALUE
				;

%%
/* -------------- body section -------------- */
void
yyerror (char const *s)
{
  fprintf (stderr, "%s\n", s);
}
