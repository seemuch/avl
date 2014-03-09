%{
#include <stdlib.h>
#include "parser.h"
%}

%option debug warn verbose

delim		[ \t]
ws		{delim}+
digit		[0-9]

%%

{ws}		{}
{digit}		{ yylval = atoi(yytext); return DIGIT; }

%%

void yyerror(const char *s)
{
	fprintf(stderr, "%s\n", s);
}

int yywrap()
{
	return 1;
}
