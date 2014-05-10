#ifndef SYNTAX_TREE_H_
#define SYNTAX_TREE_H_

typedef enum {
	INTCON_NODE,
	BOOLCON_NODE,
	CHARCON_NODE,
	STRLIT_NODE,
	VARTYPE_NODE,
	ID_NODE,
	MATHOP_NODE,
	OPERATOR_NODE
} nodeTypeEnum;

typedef enum {
	parentheses_exp,
	array,
	func_call,
	concatenate,
	math_op,
	len,
	assignment,
	disp_exp,
	hide_exp,
	swap,
	print,
	print_list,
	var_decl,
	var_decl_disp,
	var_decl_hide,
	arr_decl,
	init_list,
	empty_state,
	exp_state,
	declar_state,
	comp_state,
	state_list,
	display_state,
	select_state,
	while_state,
	do_while_state,
	for_state,
	jump_continue_state,
	jump_break_state,
	jump_ret_state,
	trans_unit,
	func_def,
	para_declar
} operatorTypeEnum;

typedef enum {
	VOID_TYPE,
	CHAR_TYPE,
	INT_TYPE,
	STRING_TYPE,
	INDEX_TYPE,
	BOOL_TYPE
} varTypeEnum;

// integer constants
typedef struct {
	int value;
} intConNode;

//char constants
typedef struct {
	char* value;
} charConNode;

// bool constants
typedef struct {
	int value;
} boolConNode;

// string literal
typedef struct {
	char* value;
} strLitNode;

// type specifier
typedef struct {
	varTypeEnum value;
} varTypeNode;

// identifiers
typedef struct {
	char* value;
} idNode;

// math operator
typedef struct {
	char* value;
} mathOpNode;

// operators
typedef struct {
	operatorTypeEnum opType;
	int numOperands;
	struct nodeTypeTag **op;
} oprNode;


//////////////////////////////////////////////////////////

typedef struct nodeTypeTag {
	nodeTypeEnum type;

	union {
		intConNode intCon;
		charConNode charCon;
		boolConNode boolCon;
		strLitNode strLit;
		varTypeNode varType;
		idNode id;
		mathOpNode mathOp;
		oprNode opr;
		
	};

} nodeType;

///////////////////////////////////////////////////////////

nodeType* intConNodeCreator (int value);

nodeType* charConNodeCreator (char* value);

nodeType* boolConNodeCreator (int value);

nodeType* strLitNodeCreator (char* value);

nodeType* varTypeNodeCreator (varTypeEnum type);

nodeType* idNodeCreator (char* value);

nodeType* mathOpNodeCreator(char* value);

nodeType* operatorNodeCreator (operatorTypeEnum, int, ...);

void avl_code_generator(nodeType* root);

#endif
