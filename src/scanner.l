%{
#include <stdlib.h>
#include "parser.h"

void yyerror();
%}

%option debug warn verbose

D                   [0-9]
L                   [a-zA-Z_]

%x COMMENT

%%

"/*"                { BEGIN(COMMENT); }
<COMMENT>"*/"       { BEGIN(INITIAL); }
<COMMENT>.          { /* disregard all characters in comments */ }

"bool"              { yylval.strLitVal = strdup(yytext); return BOOL; }
"break"             { yylval.strLitVal = strdup(yytext); return BREAK; }
"char"              { yylval.strLitVal = strdup(yytext); return CHAR; }
"continue"          { yylval.strLitVal = strdup(yytext); return CONTINUE; }
"display"           { yylval.strLitVal = strdup(yytext); return DISPLAY; }
"do"                { yylval.strLitVal = strdup(yytext); return DO; }
"else"              { yylval.strLitVal = strdup(yytext); return ELSE; }
"for"               { yylval.strLitVal = strdup(yytext); return FOR; }
"hide"              { yylval.strLitVal = strdup(yytext); return HIDE; }
"if"                { yylval.strLitVal = strdup(yytext); return IF; }
"index"             { yylval.strLitVal = strdup(yytext); return INDEX; }
"int"               { yylval.strLitVal = strdup(yytext); return INT; }
"len"               { yylval.strLitVal = strdup(yytext); return LEN; }
"print"             { yylval.strLitVal = strdup(yytext); return PRINT; }
"return"            { yylval.strLitVal = strdup(yytext); return RETURN; }
"swap"              { yylval.strLitVal = strdup(yytext); return SWAP; }
"void"              { yylval.strLitVal = strdup(yytext); return VOID; }
"while"             { yylval.strLitVal = strdup(yytext); return WHILE; }
"<begin_display>"   { yylval.strLitVal = strdup(yytext); return BEGIN_DISPLAY; }
"<end_display>"     { yylval.strLitVal = strdup(yytext); return END_DISPLAY; }

{L}({L}|{D})*       { yylval.strLitVal = strdup(yytext); return IDENTIFIER; }
{D}+                { yylval.strLitVal = strdup(yytext); return CONSTANT; }

'(\\.|[^\\'])+'     { yylval.strLitVal = strdup(yytext); return CONSTANT; }

\"(\\.|[^\\"])*\"   { yylval.strLitVal = strdup(yytext); return STRING_LITERAL; }

"++"                { yylval.strLitVal = strdup(yytext); return INC_OP; }
"--"                { yylval.strLitVal = strdup(yytext); return DEC_OP; }
"&&"                { yylval.strLitVal = strdup(yytext); return AND_OP; }
"||"                { yylval.strLitVal = strdup(yytext); return OR_OP; }
"<="                { yylval.strLitVal = strdup(yytext); return LE_OP; }
">="                { yylval.strLitVal = strdup(yytext); return GE_OP; }
"=="                { yylval.strLitVal = strdup(yytext); return EQ_OP; }
"!="                { yylval.strLitVal = strdup(yytext); return NE_OP; }
";"                 { yylval.strLitVal = strdup(yytext); return ';'; }
"{"                 { yylval.strLitVal = strdup(yytext); return '{'; }
"}"                 { yylval.strLitVal = strdup(yytext); return '}'; }
","                 { yylval.strLitVal = strdup(yytext); return ','; }
":"                 { yylval.strLitVal = strdup(yytext); return ':'; }
"="                 { yylval.strLitVal = strdup(yytext); return '='; }
"("                 { yylval.strLitVal = strdup(yytext); return '('; }
")"                 { yylval.strLitVal = strdup(yytext); return ')'; }
"["                 { yylval.strLitVal = strdup(yytext); return '['; }
"]"                 { yylval.strLitVal = strdup(yytext); return ']'; }
"!"                 { yylval.strLitVal = strdup(yytext); return '!'; }
"-"                 { yylval.strLitVal = strdup(yytext); return '-'; }
"+"                 { yylval.strLitVal = strdup(yytext); return '+'; }
"*"                 { yylval.strLitVal = strdup(yytext); return '*'; }
"/"                 { yylval.strLitVal = strdup(yytext); return '/'; }
"%"                 { yylval.strLitVal = strdup(yytext); return '%'; }
"<"                 { yylval.strLitVal = strdup(yytext); return '<'; }
">"                 { yylval.strLitVal = strdup(yytext); return '>'; }
"?"                 { yylval.strLitVal = strdup(yytext); return '?'; }

[ \t\v\n\f]         { /* disregard white spaces */ }
.                   { yyerror(yytext); }

%%

int yywrap()
{
	return 1;
}

void yyerror(const char *s)
{
	fprintf(stderr, "%s\n", s);
}
