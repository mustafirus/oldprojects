%{
#include "stdafx.h"
#include "DevFrame.h"

typedef string YYSTYPE;
#define YYSTYPE_IS_DECLARED 1
#include "FrameParser.hpp"

int yylex ();
void yyerror (char const *s);
%}

%token FRAME_TYPE FIELD_NAME
%start frame
%defines

/* -------------- rules section -------------- */
%%
frames 	: frame
		| frames frame
		;
fields 	: field
		| fields ',' field
		;
frame	: '<' FRAME_TYPE { DevFrame::begin_add_frame($2); } '>'
		{ DevFrame::end_add_frame(); }
		| '<' FRAME_TYPE { DevFrame::begin_add_frame($2);} frames '>'
		{ DevFrame::end_add_frame(); }
		| '<' FRAME_TYPE { DevFrame::begin_add_frame($2);} fields '>'
		{ DevFrame::end_add_frame(); }
		;
field 	: FIELD_NAME
		{ DevFrame::add_field($1);}
		;
%%
/* -------------- body section -------------- */
void
yyerror (char const *s)
{
  fprintf (stderr, "%s\n", s);
}