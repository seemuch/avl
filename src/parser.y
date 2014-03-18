%{
#include <stdio.h>

int yylex();
void yyerror(const char *msg);
%}

%debug
%verbose

%token DIGIT

%%

program:
       	DIGIT '\n'	{ printf("%d\n", $1); }

%%
