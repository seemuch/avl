%{
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <stdarg.h>
#include "include.h"

using std::string;

nodeType* intConNode (int val);
nodeType* strLitNode (string str);
nodeType* varTypeNode(string type);
nodeType* idNode (string value);

nodeType* operatorNodeCreator (int, int, ...);

/*
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
*/

int yylex();
void yyerror(const char *msg);
%}

%debug

%union {
	int intConVal;
	string strLitVal;
	string idVal;

	nodeType* nt;
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
	| CONSTANT 							{ $<nt>$ = intConNode ($<nt>1); }
	| STRING_LITERAL 					{ $<nt>$ = strLitNode ($<nt>1); }
	| '(' conditional_expression ')'
	;

postfix_expression
	: primary_expression
	| postfix_expression '[' conditional_expression ']'								{ $<nt>$ = operatorNodeCreator (array, 2, $<nt>1, $<nt>3);}
	| postfix_expression '[' conditional_expression ':' conditional_expression ']'  { $<nt>$ = operatorNodeCreator (array, 3, $<nt>1, $<nt>3, $<nt>5);}
	| postfix_expression '(' ')'													{ $<nt>$ = operatorNodeCreator (func_cal, 1, $<nt>1);}
	| postfix_expression '(' argument_expression_list ')' 							{ $<nt>$ = operatorNodeCreator (func_cal, 1, $<nt>1);}	
	| postfix_expression INC_OP														{ $<nt>$ = operatorNodeCreator (inc_op_post, 1, $<nt>1);}
	| postfix_expression DEC_OP														{ $<nt>$ = operatorNodeCreator (dec_op_post, 1, $<nt>1);}
	;

argument_expression_list
	: conditional_expression								{ $<nt>$ = $<nt>1;}
	| argument_expression_list ',' conditional_expression 	{ $<nt>$ = operatorNodeCreator( concatinate, 2, $<nt>1, $<nt>3);}
	;

unary_expression
	: postfix_expression				{$<nt>$ = $<nt>1;}
	| INC_OP unary_expression			{$<nt>$ = operatorNodeCreator (inc_op_pre,1, $<nt>2 );}
	| DEC_OP unary_expression			{$<nt>$ = operatorNodeCreator (dec_op_pre,1, $<nt>2 );}
	| unary_operator cast_expression
	| LEN '(' unary_expression ')'		{$<nt>$ = operatorNode( len, 1, $<nt>3 )};
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
	: VOID 						{ $<nt>$ = varTypeNodeCrator(0); }
	| CHAR 						{ $<nt>$ = varTypeNodeCrator(1); }
	| INT 						{ $<nt>$ = varTypeNodeCrator(2); }
	| STRING 					{ $<nt>$ = varTypeNodeCrator(3); }
	| INDEX 					{ $<nt>$ = varTypeNodeCrator(4); }
	| BOOL 						{ $<nt>$ = varTypeNodeCrator(5); }
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
	: type_specifier init_declarator 				{ $<nt>$ = operatorNodeCreator (variableDeclaration, 3, 0, $<nt>1, $<nt>2);  } 
	| DISPLAY type_specifier init_declarator 		{ $<nt>$ = operatorNodeCreator (variableDeclaration, 3, 1, $<nt>2, $<nt>3);  }
	| HIDE type_specifier init_declarator 			{ $<nt>$ = operatorNodeCreator (variableDeclaration, 3, 0, $<nt>2, $<nt>3);  }
	;

init_declarator
	: declarator 									{ $<nt>$ = $<nt>1; }
	| declarator '=' initializer 					{ $<nt>$ = operatorNodeCreator(assignment, 2, $<nt>1, $<nt>3); }
	;

declarator
	: IDENTIFIER 									{ $<nt>$ = idNodeCreator($<nt>1); }
	| IDENTIFIER '[' conditional_expression ']' 	{ $<nt>$ = operatorNodeCreator(arrayDeclaration, 2, $<nt>1, $<nt>3); }
	| IDENTIFIER '[' ']' 							{ $<nt>$ = operatorNodeCreator(arrayDeclaration, 1, $<nt>1); }
	;

initializer
	: conditional_expression 						{ $<nt>$ = $<nt>1; }
	| '{' initializer_list '}' 						{ $<nt>$ = $<nt>2; }
	;

initializer_list
	: conditional_expression 						{ $<nt>$ = $<nt>1; }
	| initializer_list ',' conditional_expression 	{ $<nt>$ = operatorNodeCreator(concatenate, 2, $<nt>1, $<nt>3); }
	;

statement
	: compound_statement							{ $<nt>$ = $<nt>1; }
	| expression_statement							{ $<nt>$ = $<nt>1; }
	| declaration_statement							{ $<nt>$ = $<nt>1; }
	| display_statement								{ $<nt>$ = $<nt>1; }
	| selection_statement							{ $<nt>$ = $<nt>1; }
	| iteration_statement							{ $<nt>$ = $<nt>1; }
	| jump_statement								{ $<nt>$ = $<nt>1; }
	;

expression_statement
	: expression ';'								{ $<nt>$ = operatorNodeCreator(exp_state, 1, $<nt>1); }
	;

declaration_statement
	: declaration ';'								{ $<nt>$ = operatorNodeCreator(declar_state, 1, $<nt>1); }
	;

compound_statement 
	: '{' '}'										{ $<nt>$ = operatorNodeCreator(comp_state, 0); }
	| '{' statement_list '}'						{ $<nt>$ = operatorNodeCreator(comp_state, 1, $<nt>2); }
	;

statement_list
	: statement										{ $<nt>$ = $<nt>1; }
	| statement_list statement						{ $<nt>$ = operatorNodeCreator(state_list, 2, $<nt>1, $<nt>2); }
	;

display_statement
	: BEGIN_DISPLAY statement_list END_DISPLAY		{ $<nt>$ = operatorNodeCreator(display_state, 1, $<nt>2); }
	;

selection_statement
	: IF '(' conditional_expression ')' statement	{ $<nt>$ = operatorNodeCreator(select_state, 2, $<nt>3, $<nt>5); }
	| IF '(' conditional_expression ')' statement	{ $<nt>$ = operatorNodeCreator(select_state, 3, $<nt>3, $<nt>5, $<nt>7); }
      ELSE statement
	;

iteration_statement
	: WHILE '(' conditional_expression ')' statement			{ $<nt>$ = operatorNodeCreator(iter_state, 2, $<nt>3, $<nt>5); }
	| DO statement WHILE '(' conditional_expression ')' ';'		{ $<nt>$ = operatorNodeCreator(iter_state, 2, $<nt>2, $<nt>5); }
	| FOR '(' expression ';' conditional_expression ';' ')'		{ $<nt>$ = operatorNodeCreator(iter_state, 3, $<nt>3, $<nt>5, $<nt>8); }
	      statement
	| FOR '(' expression ';' conditional_expression ';'			{ $<nt>$ = operatorNodeCreator(iter_state, 4, $<nt>3, $<nt>5, $<nt>7, $<nt>9); }
	      expression ')' statement
	| FOR '(' declaration ';' conditional_expression ';' ')'	{ $<nt>$ = operatorNodeCreator(iter_state, 3, $<nt>3, $<nt>5, $<nt>8); }
	      statement
	| FOR '(' declaration ';' conditional_expression ';'		{ $<nt>$ = operatorNodeCreator(iter_state, 4, $<nt>3, $<nt>5, $<nt>7, $<nt>9); }
	      expression ')' statement
	;

jump_statement
	: CONTINUE ';'									{ $<nt>$ = operatorNodeCreator(jump_continue_state, 0); }
	| BREAK ';'										{ $<nt>$ = operatorNodeCreator(jump_break_state, 0); }
	| RETURN ';'									{ $<nt>$ = operatorNodeCreator(jump_ret_state, 0); }
	| RETURN conditional_expression ';'				{ $<nt>$ = operatorNodeCreator(jump_ret_state, 1, $<nt>2); }
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

///////////////////////////////////////////////////////////////

nodeType* intConNodeCreator (int value) {
	nodeType *p;

	// allocating memory
	if ((p = malloc(sizeof(nodeType))) == NULL)
		yyerror("out of memory");

	p->type = INTCON;
	p->intCon.value = value;

	return p;
}

///////////////////////////////////////////////////////////////

nodeType* strLitNodeCreator (string value) {
	nodeType* p;

	// allocating memory
	if ((p = malloc(sizeof(nodeType))) == NULL)
		yyerror("out of memory");
	
	p->type = STRLIT;
	p->strLit.value = value;

	return p;
}

///////////////////////////////////////////////////////////////

nodeType* varTypeNodeCrator (int type) {
	nodeType* p;

	// allocating memory
	if ((p = malloc(sizeof(nodeType))) == NULL)
		yyerror("out of memory");
	
	p->type = VARTYPE;
	p->varType.value = type;

	return p;
}

///////////////////////////////////////////////////////////////

nodeType* idNodeCreator (string value) {
	nodeType* p;

	// allocating memory
	if ((p = malloc(sizeof(nodeType))) == NULL)
		yyerror("out of memory");

	p->type = ID; 
	p->id.value = value;

	return p;
}

///////////////////////////////////////////////////////////////

nodeType* unaryNodeCreater(char value) {
	nodeType* p;

	// allocating memroy
	if ((p = malloc(sizeof(nodeType))) == NULL)
		yyerror("out of memory");

	p->type = UNARY; 
	p->unary.value = value;

	return p;
}

///////////////////////////////////////////////////////////////

nodeType* operatorNodeCreator (operatorTypeEnum operator, int numOperands, ...) {
	nodeType* p;
	va_list ap;

	// allocating memory
	if ((p = malloc(sizeof(nodeType))) == NULL)
		yyerror("out of memory");
	
	p->type = OPERATOR;
	p->opr.opType = operator;
	p->opr.op = vector<nodeType*> (numOperands, 0);

	va_start(ap, numOperands);
	for (int i = 0; i < numOperands; i ++) {
		p->opr.op[i] = va_arg(vl, nodeType*);
	}
	va_and(ap);

	return p;
}
