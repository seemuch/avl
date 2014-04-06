%{
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <vector>
#include "include.h"

using std::string;

nodeType* intConNode (int val);
nodeType* strLitNode (string str);
nodeType* varTypeNode(string type);
nodeType* idNode (string value);

typedef struct {
	typeEnum type;
	union {
		int intValue;
		char charValue;
		bool boolValue;
		string stringvalue;
	} value;

} variableTokenValue;

typedef typename unordered_map<string, const nodeType*> symbolTable;
vector<symbolTable> symStack;

symbolTable global;
symStack.push_back(global);

int yylex();
void yyerror(const char *msg);
%}

%debug

%union {
	int intConVal;
	string strLitVal;
	string idVal;
}

%token <idVal> 		IDENTIFIER LEN
%token <intConVal> 	CONSTANT 
%token <strLitVal> 	STRING_LITERAL 

%token INC_OP DEC_OP LE_OP GE_OP EQ_OP NE_OP
%token AND_OP OR_OP

%token PRINT SWAP 

%token CHAR INT VOID BOOL INDEX STRING
%token DISPLAY HIDE

%token IF ELSE WHILE DO FOR CONTINUE BREAK RETURN END_DISPLAY BEGIN_DISPLAY

%start translation_unit

%%

primary_expression
	: IDENTIFIER
	| CONSTANT 							{ $$ = intConNode ($1); }
	| STRING_LITERAL 					{ $$ = strLitNode ($1); }
	| '(' conditional_expression ')'
	;

postfix_expression
	: primary_expression
	| postfix_expression '[' conditional_expression ']'
	| postfix_expression '[' conditional_expression ':'
	      conditional_expression ']'
	| postfix_expression '(' ')'
	| postfix_expression '(' argument_expression_list ')'
	| postfix_expression INC_OP
	| postfix_expression DEC_OP
	;

argument_expression_list
	: conditional_expression
	| argument_expression_list ',' conditional_expression
	;

unary_expression
	: postfix_expression
	| INC_OP unary_expression
	| DEC_OP unary_expression
	| unary_operator cast_expression
	| LEN '(' unary_expression ')'
	;

unary_operator
	: '+'
	| '-'
	| '!'
	;

cast_expression
	: unary_expression
	| '(' type_specifier ')' cast_expression
	;

multiplicative_expression
	: cast_expression
	| multiplicative_expression '*' cast_expression
	| multiplicative_expression '/' cast_expression
	| multiplicative_expression '%' cast_expression
	;

additive_expression
	: multiplicative_expression
	| additive_expression '+' multiplicative_expression
	| additive_expression '-' multiplicative_expression
	;

relational_expression
	: additive_expression
	| relational_expression '<' additive_expression
	| relational_expression '>' additive_expression
	| relational_expression LE_OP additive_expression
	| relational_expression GE_OP additive_expression
	;

equality_expression
	: relational_expression
	| equality_expression EQ_OP relational_expression
	| equality_expression NE_OP relational_expression
	;

logical_and_expression
	: equality_expression
	| logical_and_expression AND_OP equality_expression
	;

logical_or_expression
	: logical_and_expression
	| logical_or_expression OR_OP logical_and_expression
	;

conditional_expression
	: logical_or_expression
	| logical_or_expression '?' conditional_expression ':' 
	      conditional_expression
	;

assignment_expression
	: postfix_expression '=' conditional_expression
	;

type_specifier
	: VOID 						{ $$ = varTypeNode("void"); }
	| CHAR 						{ $$ = varTypeNode("char"); }
	| INT 						{ $$ = varTypeNode("int"); }
	| STRING 					{ $$ = varTypeNode("string"); }
	| INDEX 					{ $$ = varTypeNode("index"); }
	| BOOL 						{ $$ = varTypeNode("bool"); }
	| type_specifier '[' ']'
	;

expression
	: conditional_expression 
	| assignment_expression 
	| DISPLAY IDENTIFIER 
	| HIDE IDENTIFIER 
	| SWAP '(' IDENTIFIER ',' conditional_expression ','
	      conditional_expression ')' 
	| PRINT argument_expression_list 
	;

declaration
	: type_specifier init_declarator_list 			{ $$ = variableIdNode ($1, $2, 0); } 
	| DISPLAY type_specifier init_declarator_list   { $$ = variableIdNode ($1, $2, 1); }
	| HIDE type_specifier init_declarator_list 		{ $$ = variableIdNode ($1, $2, -1); }
	;

init_declarator_list
	: init_declarator 								
	| init_declarator_list ',' init_declarator
	;

init_declarator
	: declarator
	| declarator '=' initializer
	;

declarator
	: IDENTIFIER 									{ $$ = idNode($1); }
	| IDENTIFIER '[' conditional_expression ']' 	
	| IDENTIFIER '[' ']'
	;

initializer
	: conditional_expression
	| '{' initializer_list '}'
	;

initializer_list
	: conditional_expression
	| initializer_list ',' conditional_expression
	;

statement
	: compound_statement
	| expression_statement
	| declaration_statement
	| display_statement
	| selection_statement
	| iteration_statement
	| jump_statement
	;

expression_statement
	: expression ';'
	;

declaration_statement
	: declaration ';'
	;

compound_statement 
	: '{' '}'
	| '{' statement_list '}'
	;

statement_list
	: statement
	| statement_list statement
	;

display_statement
	: BEGIN_DISPLAY statement_list END_DISPLAY
	;

selection_statement
	: IF '(' conditional_expression ')' statement
	| IF '(' conditional_expression ')' statement
      ELSE statement
	;

iteration_statement
	: WHILE '(' conditional_expression ')' statement
	| DO statement WHILE '(' conditional_expression ')' ';'
	| FOR '(' expression ';' conditional_expression ';' ')'
	      statement
	| FOR '(' expression ';' conditional_expression ';'
	      expression ')' statement
	| FOR '(' declaration ';' conditional_expression ';' ')'
	      statement
	| FOR '(' declaration ';' conditional_expression ';'
	      expression ')' statement
	;

jump_statement
	: CONTINUE ';'
	| BREAK ';'
	| RETURN ';'
	| RETURN conditional_expression ';'
	;

translation_unit
	: function_definition	
	| translation_unit function_definition
	;

function_definition
	: type_specifier  IDENTIFIER '(' parameter_list ')'
	      compound_statement
	| type_specifier  IDENTIFIER '('  ')' compound_statement
	;

parameter_list
	: parameter_declaration
	| parameter_list ',' parameter_declaration
	;

parameter_declaration
	: type_specifier declarator
	;


%%

nodeType* intConNode (int value) {
	nodeType *p;

	// allocating memory
	if ((p = malloc(sizeof(nodeType))) == NULL)
		yyerror("out of memory");

	p->type = intCon;
	p->intCon.value = value;

	return p;
}

nodeType* strLitNode (string value) {
	nodeType* p;

	// allocating memory
	if ((p = malloc(sizeof(nodeType))) == NULL)
		yyerror("out of memory");
	
	p->type = strLit;
	p->strLit.value = value;

	return p;
}

nodeType* varTypeNode (string type) {
	nodeType* p;

	// allocating memory
	if ((p = malloc(sizeof(nodeType))) == NULL)
		yyerror("out of memory");
	
	p->type = varType;
	p->varType.type = type;

	return p;

}

nodeType* idNode(string value) {
	nodeType* p;

	// allocating memroy
	if ((p = malloc(sizeof(nodeType))) == NULL)
		yyerror("out of memory");

	p->type = idType; 
	p->id.name = value;

	
}

nodeType* variableIdNode (nodeType* type, nodeType* id, int display) {
	nodeType* p;

	// allocating memory
	if ((p = malloc(sizeof(nodeType))) == NULL)
		yyerror("out of memory");
	

}
