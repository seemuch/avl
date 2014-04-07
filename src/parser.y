%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "st_list.h"

extern FILE *yyout;

int yylex();
void yyerror(const char *msg);
char *concatenate(int num_args, ...);
char *concat_nospace(int num_args, ...);
void preprocess();
void postprocess();

/* a list of symbol tables */
struct st_list *stables = NULL;
%}

%debug

%union {
    int intConVal;
    char *strLitVal;
    char *idVal;
}

%token IDENTIFIER CONSTANT STRING_LITERAL LEN
%token INC_OP DEC_OP LE_OP GE_OP EQ_OP NE_OP
%token AND_OP OR_OP

%token PRINT SWAP 

%token CHAR INT VOID BOOL INDEX STRING

%token DISPLAY HIDE

%token IF ELSE WHILE DO FOR CONTINUE BREAK RETURN END_DISPLAY BEGIN_DISPLAY

%start program

%type<strLitVal> enter_scope
%type<strLitVal> leave_scope

%type<strLitVal> primary_expression
%type<strLitVal> postfix_expression
%type<strLitVal> argument_expression_list
%type<strLitVal> unary_expression
%type<strLitVal> cast_expression
%type<strLitVal> multiplicative_expression
%type<strLitVal> additive_expression
%type<strLitVal> relational_expression
%type<strLitVal> equality_expression
%type<strLitVal> logical_and_expression
%type<strLitVal> logical_or_expression
%type<strLitVal> conditional_expression
%type<strLitVal> assignment_expression
%type<strLitVal> expression

%type<strLitVal> type_specifier
%type<strLitVal> declaration
%type<strLitVal> init_declarator
%type<strLitVal> declarator
%type<strLitVal> initializer
%type<strLitVal> initializer_list
%type<strLitVal> init_declarator_list

%type<strLitVal> statement
%type<strLitVal> expression_statement
%type<strLitVal> declaration_statement
%type<strLitVal> compound_statement
%type<strLitVal> statement_list
%type<strLitVal> display_statement
%type<strLitVal> selection_statement
%type<strLitVal> iteration_statement
%type<strLitVal> jump_statement

%type<strLitVal> translation_unit
%type<strLitVal> function_definition
%type<strLitVal> parameter_list
%type<strLitVal> parameter_declaration
%type<strLitVal> program

%type<strLitVal> unary_operator

%type<strLitVal> '(' ')' '[' ']' '{' '}'
%type<strLitVal> '+' '-' '*' '/' '%'
%type<strLitVal> '<' '>' '='
%type<strLitVal> ';' ':' '?' ',' '!'
%type<strLitVal> IDENTIFIER CONSTANT STRING_LITERAL LEN
%type<strLitVal> INC_OP DEC_OP LE_OP GE_OP EQ_OP NE_OP
%type<strLitVal> AND_OP OR_OP
%type<strLitVal> PRINT SWAP 
%type<strLitVal> CHAR INT VOID BOOL INDEX STRING
%type<strLitVal> DISPLAY HIDE
%type<strLitVal> IF ELSE WHILE DO FOR CONTINUE BREAK RETURN END_DISPLAY BEGIN_DISPLAY

%%

enter_scope
    : '{'   {
                struct sym_table *st = (struct sym_table *)malloc(sizeof(struct sym_table));
                sym_table_init(st);
                st_list_add(stables, st);
            }
    ;

leave_scope
    : '}'   {
                struct sym_table *st = st_list_head(stables);
                sym_table_destroy(st);
                free(st);
                st_list_del(stables);
            }
    ;

primary_expression
    : IDENTIFIER                        { $$ = concatenate(1, $1); }
    | CONSTANT                          { $$ = concatenate(1, $1); }
    | STRING_LITERAL                    { $$ = concatenate(1, $1); }
    | '(' conditional_expression ')'    { $$ = concatenate(3, $1, $2, $3); }
    ;

postfix_expression
    : primary_expression                                    { $$ = concatenate(1, $1); }
    | postfix_expression '[' conditional_expression ']'     { $$ = concatenate(4, $1, $2, $3, $4); }
    | postfix_expression '[' conditional_expression
        ':' conditional_expression ']'                      { $$ = concatenate(5, $1, $2, $3, $4, $5); }
    | postfix_expression '(' ')'                            { $$ = concatenate(3, $1, $2, $3); }
    | postfix_expression '(' argument_expression_list ')'   { $$ = concatenate(4, $1, $2, $3, $4); }
    | postfix_expression INC_OP                             { $$ = concatenate(2, $1, $2); }
    | postfix_expression DEC_OP                             { $$ = concatenate(2, $1, $2); }
    ;

argument_expression_list
    : conditional_expression                                { $$ = concatenate(1, $1); }
    | argument_expression_list ',' conditional_expression   { $$ = concatenate(3, $1, $2, $3); }
    ;

unary_expression
    : postfix_expression                { $$ = concatenate(1, $1); }
    | INC_OP unary_expression           { $$ = concatenate(2, $1, $2); }
    | DEC_OP unary_expression           { $$ = concatenate(2, $1, $2); }
    | unary_operator cast_expression    { $$ = concatenate(2, $1, $2); }
    | LEN '(' unary_expression ')'      { $$ = concat_nospace(2, $3, strdup(".size()")); free($1); free($2); free($4); }
    ;

unary_operator
    : '+'   { $$ = concatenate(1, $1); }
    | '-'   { $$ = concatenate(1, $1); }
    | '!'   { $$ = concatenate(1, $1); }
    ;

cast_expression
    : unary_expression                          { $$ = concatenate(1, $1); }
    | '(' type_specifier ')' cast_expression    { $$ = concatenate(4, $1, $2, $3, $4); }
    ;

multiplicative_expression
    : cast_expression                                   { $$ = concatenate(1, $1); }
    | multiplicative_expression '*' cast_expression     { $$ = concatenate(3, $1, $2, $3); }
    | multiplicative_expression '/' cast_expression     { $$ = concatenate(3, $1, $2, $3); }
    | multiplicative_expression '%' cast_expression     { $$ = concatenate(3, $1, $2, $3); }
    ;

additive_expression
    : multiplicative_expression                         { $$ = concatenate(1, $1); }
    | additive_expression '+' multiplicative_expression { $$ = concatenate(3, $1, $2, $3); }
    | additive_expression '-' multiplicative_expression { $$ = concatenate(3, $1, $2, $3); }
    ;

relational_expression
    : additive_expression                               { $$ = concatenate(1, $1); }
    | relational_expression '<' additive_expression     { $$ = concatenate(3, $1, $2, $3); }
    | relational_expression '>' additive_expression     { $$ = concatenate(3, $1, $2, $3); }
    | relational_expression LE_OP additive_expression   { $$ = concatenate(3, $1, $2, $3); }
    | relational_expression GE_OP additive_expression   { $$ = concatenate(3, $1, $2, $3); }
    ;

equality_expression
    : relational_expression                             { $$ = concatenate(1, $1); }
    | equality_expression EQ_OP relational_expression   { $$ = concatenate(3, $1, $2, $3); }
    | equality_expression NE_OP relational_expression   { $$ = concatenate(3, $1, $2, $3); }
    ;

logical_and_expression
    : equality_expression                               { $$ = concatenate(1, $1); }
    | logical_and_expression AND_OP equality_expression { $$ = concatenate(3, $1, $2, $3); }
    ;

logical_or_expression
    : logical_and_expression                                { $$ = concatenate(1, $1); }
    | logical_or_expression OR_OP logical_and_expression    { $$ = concatenate(3, $1, $2, $3); }
    ;

conditional_expression
    : logical_or_expression                                 { $$ = concatenate(1, $1); }
    | logical_or_expression '?' conditional_expression ':' 
          conditional_expression                            { $$ = concatenate(5, $1, $2, $3, $4, $5); }
    ;

assignment_expression
    : postfix_expression '=' conditional_expression     { $$ = concatenate(3, $1, $2, $3); }
    ;

type_specifier
    : VOID                      { $$ = concatenate(1, $1); }
    | CHAR                      { $$ = concatenate(1, $1); }
    | INT                       { $$ = concatenate(1, strdup("AvlInt")); free($1); }
    | STRING                    { $$ = concatenate(1, $1); }
    | INDEX                     { $$ = concatenate(1, $1); }
    | BOOL                      { $$ = concatenate(1, $1); }
    | type_specifier '[' ']'    { $$ = concatenate(3, $1, $2, $3); }
    ;

expression
    : conditional_expression                                { $$ = concatenate(1, $1); }
    | assignment_expression                                 { $$ = concatenate(1, $1); }
    | DISPLAY IDENTIFIER                                    { $$ = concatenate(1, $1); }
    | HIDE IDENTIFIER                                       { $$ = concatenate(2, $1, $2); }
    | SWAP '(' IDENTIFIER ',' conditional_expression ','
          conditional_expression ')'                        { $$ = concatenate(8, $1, $2, $3, $4, $5, $6, $7, $8); }
    | PRINT argument_expression_list                        { $$ = concatenate(2, $1, $2); }
    ;

declaration
    : type_specifier init_declarator_list           { $$ = concatenate(2, $1, $2); }
    | DISPLAY type_specifier init_declarator_list   { $$ = concatenate(3, $1, $2, $3); }
    | HIDE type_specifier init_declarator_list      { $$ = concatenate(3, $1, $2, $3); }
    ;

init_declarator_list
    : init_declarator                           { $$ = concatenate(1, $1); }
    | init_declarator_list ',' init_declarator  { $$ = concatenate(3, $1, $2, $3); }
    ;

init_declarator
    : declarator                    { $$ = concatenate(1, $1); }
    | declarator '=' initializer    { $$ = concatenate(3, $1, $2, $3); }
    ;

declarator
    : IDENTIFIER                                    { $$ = concatenate(1, $1); }
    | IDENTIFIER '[' conditional_expression ']'     { $$ = concatenate(4, $1, $2, $3, $4); }
    | IDENTIFIER '[' ']'                            { $$ = concatenate(3, $1, $2, $3); }
    ;

initializer
    : conditional_expression                        { $$ = concatenate(1, $1); }
    | enter_scope initializer_list leave_scope      { $$ = concatenate(3, $1, $2, $3); }
    ;

initializer_list
    : conditional_expression                        { $$ = concatenate(1, $1); }
    | initializer_list ',' conditional_expression   { $$ = concatenate(3, $1, $2, $3); }
    ;

statement
    : compound_statement        { $$ = concatenate(1, $1); }
    | expression_statement      { $$ = concatenate(1, $1); }
    | declaration_statement     { $$ = concatenate(1, $1); }
    | display_statement         { $$ = concatenate(1, $1); }
    | selection_statement       { $$ = concatenate(1, $1); }
    | iteration_statement       { $$ = concatenate(1, $1); }
    | jump_statement            { $$ = concatenate(1, $1); }
    ;

expression_statement
    : expression ';'        { $$ = concatenate(3, $1, $2, strdup("\n")); }
    ;

declaration_statement
    : declaration ';'       { $$ = concatenate(3, $1, $2, strdup("\n")); }
    ;

compound_statement 
    : enter_scope leave_scope                  { $$ = concatenate(4, $1, strdup("\n"), $2, strdup("\n")); }
    | enter_scope statement_list leave_scope   { $$ = concatenate(6, $1, strdup("\n"), $2, strdup("\n"), $3, strdup("\n")); }
    ;

statement_list
    : statement                 { $$ = concatenate(1, $1); }
    | statement_list statement  { $$ = concatenate(2, $1, $2); }
    ;

display_statement
    : BEGIN_DISPLAY statement_list END_DISPLAY { $$ = concatenate(3, $1, $2, $3); }
    ;

selection_statement
    : IF '(' conditional_expression ')' statement   { $$ = concatenate(6, $1, $2, $3, $4, strdup("\n"), $5); }
    | IF '(' conditional_expression ')' statement
      ELSE statement                                { $$ = concatenate(9, $1, $2, $3, $4, strdup("\n"), strdup("\n"), $5, strdup("\n"), $6); }
    ;

iteration_statement
    : WHILE '(' conditional_expression ')' statement            { $$ = concatenate(5, $1, $2, $3, $4, $5); }
    | DO statement WHILE '(' conditional_expression ')' ';'     { $$ = concatenate(8, $1, $2, $3, $4, $5, $6, $7, strdup("\n")); }
    | FOR '(' expression ';' conditional_expression ';' ')'
          statement                                             { $$ = concatenate(8, $1, $2, $3, $4, $5, $6, $7, $8); }
    | FOR '(' expression ';' conditional_expression ';'
          expression ')' statement                              { $$ = concatenate(9, $1, $2, $3, $4, $5, $6, $7, $8, $9); }
    | FOR '(' declaration ';' conditional_expression ';' ')'
          statement                                             { $$ = concatenate(8, $1, $2, $3, $4, $5, $6, $7, $8); }
    | FOR '(' declaration ';' conditional_expression ';'
          expression ')' statement                              { $$ = concatenate(9, $1, $2, $3, $4, $5, $6, $7, $8, $9); }
    ;

jump_statement
    : CONTINUE ';'                          { $$ = concatenate(3, $1, $2, strdup("\n")); }
    | BREAK ';'                             { $$ = concatenate(3, $1, $2, strdup("\n")); }
    | RETURN ';'                            { $$ = concatenate(3, $1, $2, strdup("\n")); }
    | RETURN conditional_expression ';'     { $$ = concatenate(4, $1, $2, $3, strdup("\n")); }
    ;

translation_unit
    : function_definition                   { $$ = concatenate(1, $1); }
    | translation_unit function_definition  { $$ = concatenate(2, $1, $2); }
    ;

function_definition
    : type_specifier  IDENTIFIER '(' parameter_list ')'
          compound_statement                                    { $$ = concatenate(6, $1, $2, $3, $4, $5, $6); }
    | type_specifier  IDENTIFIER '('  ')' compound_statement    { $$ = concatenate(5, $1, $2, $3, $4, $5); }
    ;

parameter_list
    : parameter_declaration                         { $$ = concatenate(1, $1); }
    | parameter_list ',' parameter_declaration      { $$ = concatenate(3, $1, $2, $3); }
    ;

parameter_declaration
    : type_specifier declarator     { $$ = concatenate(2, $1, $2); }
    ;

program
    : translation_unit      {
                                preprocess();
                                fprintf(yyout, "%s\n", $1);
                                free($1);
								postprocess();
                            }
    ;

%%

char *concatenate(int num_args, ...)
{
	va_list ap;
	char *ret;
	int i;

	va_start(ap, num_args);

	char *first = va_arg(ap, char *);
	ret = (char *)malloc(strlen(first) + 1);
	strcpy(ret, first);
	free(first);

	for (i = 1; i < num_args; i++)
	{
		char *str = va_arg(ap, char *);
		ret = realloc(ret, strlen(ret) + strlen(str) + 2);
		strcat(ret, " ");
		strcat(ret, str);
		free(str);
	}

	va_end(ap);

	return ret;
}

char *concat_nospace(int num_args, ...)
{
	va_list ap;
	char *ret;
	int i;

	va_start(ap, num_args);

	char *first = va_arg(ap, char *);
	ret = (char *)malloc(strlen(first) + 1);
	strcpy(ret, first);
	free(first);

	for (i = 1; i < num_args; i++)
	{
		char *str = va_arg(ap, char *);
		ret = realloc(ret, strlen(ret) + strlen(str) + 1);
		strcat(ret, str);
		free(str);
	}

	va_end(ap);

	return ret; }

void print_line(const char *line)
{
	fprintf(yyout, "%s\n", line);
}

void print_header()
{
	print_line("#include <AvlVisualizer.h>");
	print_line("#include <AvlTypes.h>");
	print_line("#include <condition_variable>");
	print_line("#include <cstdlib>");
	print_line("");

	print_line("AvlVisualizer *__avl__vi = NULL;");
	print_line("bool __avl__ready() { return __avl__vi != NULL; }");
	print_line("std::mutex __avl_mtx;");
	print_line("std::condition_variable_any __avl__cv;");
	print_line("");

	print_line("void __avl__display(int argc, char **argv)");
	print_line("{");
	print_line("\t__avl_mtx.lock();");
	print_line("\t__avl__vi = new AvlVisualizer(argc, argv);");
	print_line("\t__avl__cv.notify_one();");
	print_line("\t__avl_mtx.unlock();");
	print_line("");
	print_line("\t__avl__vi->show();");
	print_line("}");
	print_line("");
}

void preprocess()
{
	print_header();

	stables = (struct st_list *)malloc(sizeof(struct st_list));
	st_list_init(stables);
}

void postprocess()
{
	st_list_destroy(stables);
	free(stables);
}
