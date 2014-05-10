%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "parser.h"

void yyerror(const char *format, ...);
%}

%option debug warn verbose

D                   [0-9]
L                   [A-Za-z_]
C                   [A-Za-z0-9,.?!:; ]

%x COMMENT

%%

"/*"                { BEGIN(COMMENT); }
<COMMENT>"*/"       { BEGIN(INITIAL); }
<COMMENT>.          { /* disregard all characters in comments. */ }

true                { return TRUE; }
false               { return FALSE; }
break               { return BREAK; }
char                { return CHAR; }
string              { return STRING; }
continue            { return CONTINUE; }
do                  { return DO; }
else                { return ELSE; }
for                 { return FOR; }
if                  { return IF; }
int                 { return INT; }
index               { return INDEX; }
return              { return RETURN; }
len                 { return LEN; }
void                { return VOID; }
while               { return WHILE; }
swap                { return SWAP; }
print               { return PRINT; }
bool                { return BOOL; }
display             { return DISPLAY; }
hide                { return HIDE; }
\<begin_display\>   { return BEGIN_DISPLAY; }
\<end_display\>     { return END_DISPLAY; }

{L}({L}|{D})*       { /* identifier */
                        yylval.idVal = strdup(yytext);
                        return IDENTIFIER;
                    }

{D}+                { /* integer */
                        yylval.intConVal = atoi(yytext);
                        return CONSTANT;
                    }

'{C}'               { /* single character, escape characters are not allowed */
                        yylval.strLitVal = strdup(yytext);
                        return CHAR_LITERAL;
                    }

\"{C}*\"            { /* string literal, escape characters are not allowed */
                        yylval.strLitVal = strdup(yytext);
                        return STRING_LITERAL;
                    }

"++"                { return INC_OP; }
"--"                { return DEC_OP; }
"&&"                { return AND_OP; }
"||"                { return OR_OP; }
"<="                { return LE_OP; }
">="                { return GE_OP; }
"=="                { return EQ_OP; }
"!="                { return NE_OP; }
";"                 { return ';'; }
"{"                 { return '{'; }
"}"                 { return '}'; }
","                 { return ','; }
":"                 { return ':'; }
"="                 { return '='; }
"("                 { return '('; }
")"                 { return ')'; }
"["                 { return '['; }
"]"                 { return ']'; }
"!"                 { return '!'; }
"-"                 { return '-'; }
"+"                 { return '+'; }
"*"                 { return '*'; }
"/"                 { return '/'; }
"%"                 { return '%'; }
"<"                 { return '<'; }
">"                 { return '>'; }
"?"                 { return '?'; }

[ \t\n]             { /* disregard white spaces */ }
.                   { yyerror("bad character found: %s", yytext); }

%%

int yywrap()
{
	return 1;
}

void yyerror(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	vfprintf(stderr, format, ap);
	fprintf(stderr, "\n");
	va_end(ap);
}
