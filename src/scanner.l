%{
#include <stdlib.h>
#include "parser.h"
void count();
void comment();
%}

%option debug warn verbose

D			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]

%%

"/*"			{ comment(); }

"break"			{ count(); return(BREAK); }
"char"			{ count(); return(CHAR); }
"continue"		{ count(); return(CONTINUE); }
"do"			{ count(); return(DO); }
"else"			{ count(); return(ELSE); }
"for"			{ count(); return(FOR); }
"if"			{ count(); return(IF); }
"int"			{ count(); return(INT); }
"index"			{ count(); return(INDEX); }
"return"		{ count(); return(RETURN); }
"len"			{ count(); return(LEN); }
"void"			{ count(); return(VOID); }
"while"			{ count(); return(WHILE); }
"swap"			{ count(); return(SWAP); }
"print"			{ count(); return(PRINT); }
"bool"			{ count(); return(BOOL); }
"display"		{ count(); return(DISPLAY); }
"hide"			{ count(); return(HIDE); }
"<begin_display>"	{ count(); return(BEGIN_DISPLAY); }
"<end_display>"		{ count(); return(END_DISPLAY); }

{L}({L}|{D})*		{ count(); return(IDENTIFIER); }

{D}+			{ count(); return(CONSTANT); }
'(\\.|[^\\'])'		{ count(); return(CONSTANT); }

\"(\\.|[^\\"])*\"	{ count(); return(STRING_LITERAL); }

"++"			{ count(); return(INC_OP); }
"--"			{ count(); return(DEC_OP); }
"&&"			{ count(); return(AND_OP); }
"||"			{ count(); return(OR_OP); }
"<="			{ count(); return(LE_OP); }
">="			{ count(); return(GE_OP); }
"=="			{ count(); return(EQ_OP); }
"!="			{ count(); return(NE_OP); }
";"			{ count(); return(';'); }
"{"			{ count(); return('{'); }
"}"			{ count(); return('}'); }
","			{ count(); return(','); }
":"			{ count(); return(':'); }
"="			{ count(); return('='); }
"("			{ count(); return('('); }
")"			{ count(); return(')'); }
"["			{ count(); return('['); }
"]"			{ count(); return(']'); }
"!"			{ count(); return('!'); }
"-"			{ count(); return('-'); }
"+"			{ count(); return('+'); }
"*"			{ count(); return('*'); }
"/"			{ count(); return('/'); }
"%"			{ count(); return('%'); }
"<"			{ count(); return('<'); }
">"			{ count(); return('>'); }
"?"			{ count(); return('?'); }

[ \t\v\n\f]		{ count(); }
.			{ /* ignore bad characters */ }



%%
int yywrap() {
	return 1;
}

void comment() {
	char c, c1;

loop:
	while ((c = input()) != '*' && c != 0)
		putchar(c);

	if ((c1 = input()) != '/' && c != 0)
	{
		unput(c1);
		goto loop;
	}

	if (c != 0)
		putchar(c1);
}

int column = 0;

void count()
{
	int i;

	for (i = 0; yytext[i] != '\0'; i++)
		if (yytext[i] == '\n')
			column = 0;
		else if (yytext[i] == '\t')
			column += 8 - (column % 8);
		else
			column++;

	ECHO;
}



void yyerror(const char *s)
{
	fprintf(stderr, "%s\n", s);
}
