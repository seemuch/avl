%{
#include <stdio.h>
%}

%debug
%verbose

%token DIGIT

%%

program:
       	DIGIT '\n'	{ printf("%d\n", $1); }

%%
