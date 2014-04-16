%{
#include <stdio.h>
#include <stdarg.h>
#include "include.h"

nodeType* intConNodeCreator (int val);
nodeType* strLitNodeCreator (char* str);
nodeType* varTypeNodeCreator (varTypeEnum type);
nodeType* idNodeCreator (char* value);

nodeType* operatorNodeCreator (operatorTypeEnum, int, ...);
nodeType* unaryNodeCreator(char);

extern ex(nodeType*);

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
	char* strLitVal;
	char* idVal;
	struct nodeTypeTag* nt;
}

%token <idVal> IDENTIFIER 
%token LEN
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
	| CONSTANT 							{ $<nt>$ = intConNodeCreator ($<intConVal>1); }
	| STRING_LITERAL 					{ $<nt>$ = strLitNodeCreator ($<strLitVal>1); }
	| '(' conditional_expression ')' 	{ $<nt>$ = $<nt>2; }
	;

postfix_expression
	: primary_expression
	| postfix_expression '[' conditional_expression ']'								{ $<nt>$ = operatorNodeCreator (array, 2, $<nt>1, $<nt>3);}
	| postfix_expression '[' conditional_expression ':' conditional_expression ']'  { $<nt>$ = operatorNodeCreator (array, 3, $<nt>1, $<nt>3, $<nt>5);}
	| postfix_expression '(' ')'													{ $<nt>$ = operatorNodeCreator (func_call, 1, $<nt>1);}
	| postfix_expression '(' argument_expression_list ')' 							{ $<nt>$ = operatorNodeCreator (func_call, 1, $<nt>1);}	
	| postfix_expression INC_OP														{ $<nt>$ = operatorNodeCreator (inc_op_post, 1, $<nt>1);}
	| postfix_expression DEC_OP														{ $<nt>$ = operatorNodeCreator (dec_op_post, 1, $<nt>1);}
	;

argument_expression_list
	: conditional_expression								{ $<nt>$ = $<nt>1;}
	| argument_expression_list ',' conditional_expression 	{ $<nt>$ = operatorNodeCreator( concatenate, 2, $<nt>1, $<nt>3);}
	;

unary_expression
	: postfix_expression				{$<nt>$ = $<nt>1;}
	| INC_OP unary_expression			{$<nt>$ = operatorNodeCreator (inc_op_pre, 1, $<nt>2 );}
	| DEC_OP unary_expression			{$<nt>$ = operatorNodeCreator (dec_op_pre, 1, $<nt>2 );}
	| unary_operator cast_expression 	{$<nt>$ = operatorNodeCreator (unary_op, 2, $<nt>1, $<nt>2); }
	| LEN '(' unary_expression ')'		{$<nt>$ = operatorNodeCreator( len, 1, $<nt>3 ); }
	;

unary_operator
	: '+' 		{ $<nt>$ = unaryNodeCreator ('+'); }
	| '-' 		{ $<nt>$ = unaryNodeCreator ('-'); }
	| '!' 		{ $<nt>$ = unaryNodeCreator ('!'); }
	;

cast_expression
	: unary_expression 							{ $<nt>$ = $<nt>1; }
	| '(' type_specifier ')' cast_expression 	{ $<nt>$ = operatorNodeCreator (cast, 2, $<nt>2, $<nt>4); }
	;

multiplicative_expression
	: cast_expression 								{ $<nt>$ = $<nt>1; }
	| multiplicative_expression '*' cast_expression { $<nt>$ = operatorNodeCreator(mult, 2, $<nt>1, $<nt>3); }
	| multiplicative_expression '/' cast_expression { $<nt>$ = operatorNodeCreator(dev, 2, $<nt>1, $<nt>3);  }
	| multiplicative_expression '%' cast_expression { $<nt>$ = operatorNodeCreator(mod, 2, $<nt>1, $<nt>3); }
	;

additive_expression
	: multiplicative_expression 						{ $<nt>$ = $<nt>1; }
	| additive_expression '+' multiplicative_expression { $<nt>$ = operatorNodeCreator(add, 2, $<nt>1, $<nt>3); }
	| additive_expression '-' multiplicative_expression { $<nt>$ = operatorNodeCreator(minus, 2, $<nt>1, $<nt>3); }
	;

relational_expression
	: additive_expression 								{ $<nt>$ = $<nt>1; }
	| relational_expression '<' additive_expression 	{ $<nt>$ = operatorNodeCreator(lt, 2, $<nt>1, $<nt>3); }
	| relational_expression '>' additive_expression 	{ $<nt>$ = operatorNodeCreator(gt, 2, $<nt>1, $<nt>3); }
	| relational_expression LE_OP additive_expression 	{ $<nt>$ = operatorNodeCreator(le, 2, $<nt>1, $<nt>3); }
	| relational_expression GE_OP additive_expression 	{ $<nt>$ = operatorNodeCreator(ge, 2, $<nt>1, $<nt>3); }
	;

equality_expression
	: relational_expression 							{ $<nt>$ = $<nt>1; }
	| equality_expression EQ_OP relational_expression 	{ $<nt>$ = operatorNodeCreator(eq, 2, $<nt>1, $<nt>3); }
	| equality_expression NE_OP relational_expression 	{ $<nt>$ = operatorNodeCreator(ne, 2, $<nt>1, $<nt>3); }
	;

logical_and_expression
	: equality_expression 								{ $<nt>$ = $<nt>1; }
	| logical_and_expression AND_OP equality_expression { $<nt>$ = operatorNodeCreator(and_op, 2, $<nt>1, $<nt>3); }
	;

logical_or_expression
	: logical_and_expression 								{ $<nt>$ = $<nt>1; }
	| logical_or_expression OR_OP logical_and_expression 	{ $<nt>$ = operatorNodeCreator(or_op, 2, $<nt>1, $<nt>3); }
	;

conditional_expression
	: logical_or_expression 								{ $<nt>$ = $<nt>1; }
	| logical_or_expression '?' conditional_expression ':' conditional_expression { $<nt>$ = operatorNodeCreator (que_col, 3, $<nt>1, $<nt>3, $<nt>5); }
	;

assignment_expression
	: postfix_expression '=' conditional_expression 	{ $<nt>$ = operatorNodeCreator(assignment, 2, $<nt>1, $<nt>3); }
	;

type_specifier
	: VOID 						{ $<nt>$ = varTypeNodeCreator(VOID_TYPE); }
	| CHAR 						{ $<nt>$ = varTypeNodeCreator(CHAR_TYPE); }
	| INT 						{ $<nt>$ = varTypeNodeCreator(INT_TYPE); }
	| STRING 					{ $<nt>$ = varTypeNodeCreator(STRING_TYPE); }
	| INDEX 					{ $<nt>$ = varTypeNodeCreator(INDEX_TYPE); }
	| BOOL 						{ $<nt>$ = varTypeNodeCreator(BOOL_TYPE); }
	;

expression
	: conditional_expression  			{ $<nt>$ = $<nt>1; }
	| assignment_expression  			{ $<nt>$ = $<nt>1; }
	| DISPLAY IDENTIFIER  				{ $<nt>$ = $<nt>1; }
	| HIDE IDENTIFIER  					{ $<nt>$ = $<nt>1; }
	| SWAP '(' IDENTIFIER ',' conditional_expression ',' conditional_expression ')' { $<nt>$ = operatorNodeCreator(swap, 3, $<nt>3, $<nt>5, $<nt>7); }
	| PRINT argument_expression_list  	{ $<nt>$ = operatorNodeCreator (print, 1, $<nt>2); }
	;

declaration
	: type_specifier init_declarator 				{ $<nt>$ = operatorNodeCreator (var_decl, 3, 0, $<nt>1, $<nt>2);  } 
	| DISPLAY type_specifier init_declarator 		{ $<nt>$ = operatorNodeCreator (var_decl, 3, 1, $<nt>2, $<nt>3);  }
	| HIDE type_specifier init_declarator 			{ $<nt>$ = operatorNodeCreator (var_decl, 3, 0, $<nt>2, $<nt>3);  }
	;

init_declarator
	: declarator 									{ $<nt>$ = $<nt>1; }
	| declarator '=' initializer 					{ $<nt>$ = operatorNodeCreator(assignment, 2, $<nt>1, $<nt>3); }
	;

declarator
	: IDENTIFIER 									{ $<nt>$ = idNodeCreator($<idVal>1); }
	| IDENTIFIER '[' conditional_expression ']' 	{ $<nt>$ = operatorNodeCreator(arr_decl, 2, $<nt>1, $<nt>3); }
	| IDENTIFIER '[' ']' 							{ $<nt>$ = operatorNodeCreator(arr_decl, 1, $<nt>1); }
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
	: IF '(' conditional_expression ')' statement					{ $<nt>$ = operatorNodeCreator(select_state, 2, $<nt>3, $<nt>5); }
	| IF '(' conditional_expression ')' statement ELSE statement	{ $<nt>$ = operatorNodeCreator(select_state, 3, $<nt>3, $<nt>5, $<nt>7); }
	;

iteration_statement
	: WHILE '(' conditional_expression ')' statement					{ $<nt>$ = operatorNodeCreator(iter_state, 2, $<nt>3, $<nt>5); }
	| DO statement WHILE '(' conditional_expression ')' ';'				{ $<nt>$ = operatorNodeCreator(iter_state, 2, $<nt>2, $<nt>5); }
	| FOR '(' expression ';' conditional_expression ';' ')'	statement	{ $<nt>$ = operatorNodeCreator(iter_state, 3, $<nt>3, $<nt>5, $<nt>8); }
	| FOR '(' expression ';' conditional_expression ';' expression ')' statement { $<nt>$ = operatorNodeCreator(iter_state, 4, $<nt>3, $<nt>5, $<nt>7, $<nt>9); }
	| FOR '(' declaration ';' conditional_expression ';' ')' statement	{ $<nt>$ = operatorNodeCreator(iter_state, 3, $<nt>3, $<nt>5, $<nt>8); }
	| FOR '(' declaration ';' conditional_expression ';' expression ')' statement	{ $<nt>$ = operatorNodeCreator(iter_state, 4, $<nt>3, $<nt>5, $<nt>7, $<nt>9); }
	;

jump_statement
	: CONTINUE ';'									{ $<nt>$ = operatorNodeCreator(jump_continue_state, 0); }
	| BREAK ';'										{ $<nt>$ = operatorNodeCreator(jump_break_state, 0); }
	| RETURN ';'									{ $<nt>$ = operatorNodeCreator(jump_ret_state, 0); }
	| RETURN conditional_expression ';'				{ $<nt>$ = operatorNodeCreator(jump_ret_state, 1, $<nt>2); }
	;

translation_unit
	: function_definition                   { $<nt>$ = $<nt>1; }
	| translation_unit function_definition  { $<nt>$ = operatorNodeCreator(trans_unit, 2, $<nt>1, $<nt>2); }
	;

function_definition
	: type_specifier  IDENTIFIER '(' parameter_list ')' compound_statement  { $<nt>$ = operatorNodeCreator(func_def, 4, $<nt>1, $<nt>2, $<nt>4, $<nt>6); }
	| type_specifier  IDENTIFIER '('  ')' compound_statement                { $<nt>$ = operatorNodeCreator(func_def, 3, $<nt>1, $<nt>2, $<nt>5); }
	;

parameter_list
	: parameter_declaration                     { $<nt>$ = $<nt>1; }
	| parameter_list ',' parameter_declaration  { $<nt>$ = operatorNodeCreator(concatenate, 2, $<nt>1, $<nt>3); }
	;

parameter_declaration
	: type_specifier declarator     			{ $<nt>$ = operatorNodeCreator(para_declar, 2, $<nt>1, $<nt>2); }
	;

program
	: translation_unit 							{ 
													$<nt>$ = operatorNodeCreator(prog_start, 1. $<nt>1);
													freeNode($<nt>1);
												}

%%

///////////////////////////////////////////////////////////////

nodeType* intConNodeCreator (int value) {
	nodeType *p;

	// allocating memory
	if ((p = (nodeType*)malloc(sizeof(nodeType))) == NULL)
		yyerror("out of memory");

	p->type = INTCON_NODE;
	p->intCon.value = value;

	return p;
}

///////////////////////////////////////////////////////////////

nodeType* strLitNodeCreator (char* value) {
	nodeType* p;

	// allocating memory
	if ((p = (nodeType*)malloc(sizeof(nodeType))) == NULL)
		yyerror("out of memory");
	
	p->type = STRLIT_NODE;
	p->strLit.value = value;

	return p;
}

///////////////////////////////////////////////////////////////

nodeType* varTypeNodeCreator (varTypeEnum type) {
	nodeType* p;

	// allocating memory
	if ((p = (nodeType*)malloc(sizeof(nodeType))) == NULL)
		yyerror("out of memory");
	
	p->type = VARTYPE_NODE;
	p->varType.value = type;

	return p;
}

///////////////////////////////////////////////////////////////

nodeType* idNodeCreator (char* value) {
	nodeType* p;

	// allocating memory
	if ((p = (nodeType*)malloc(sizeof(nodeType))) == NULL)
		yyerror("out of memory");

	p->type = ID_NODE; 
	p->id.value = value;

	return p;
}

///////////////////////////////////////////////////////////////

nodeType* unaryNodeCreater(char value) {
	nodeType* p;

	// allocating memroy
	if ((p = (nodeType*)malloc(sizeof(nodeType))) == NULL)
		yyerror("out of memory");

	p->type = UNARY_NODE; 
	p->unary.value = value;

	return p;
}

///////////////////////////////////////////////////////////////

nodeType* unaryNodeCreator(char op) {
	nodeType* p;
	p->type = UNARY_NODE;
	
	// allocating memory
	if ((p = (nodeType*)malloc(sizeof(nodeType))) == NULL)
		yyerror("out of memory");

	p->unary.value = op;
	return p;
}

///////////////////////////////////////////////////////////////

nodeType* operatorNodeCreator (operatorTypeEnum oprtr, int numOperands, ...) {
	nodeType* p;
	va_list ap;

	// allocating memory
	if ((p = (nodeType*)malloc(sizeof(nodeType))) == NULL)
		yyerror("out of memory");
	
	if ((p->opr.op = (nodeType**)malloc(numOperands * sizeof(nodeType))) == NULL)
	    yyerror("out of memory");
	
	p->type = OPERATOR_NODE;
	p->opr.opType = oprtr;

	va_start(ap, numOperands);
    int i;
	for (i = 0; i < numOperands; i ++) {
		p->opr.op[i] = va_arg(ap, nodeType*);
	}
	va_end(ap);

	return p;
}

///////////////////////////////////////////////////////////////

void freeNode(nodeType* node) {
	if (node->type != OPERATOR_NODE)
		free(node);

	else {
		int i = 0;
		for (; i < node->opr.numOperands; i ++) {
			freeNode(node->opr.op[i]);
		}
	}
	return;
}
