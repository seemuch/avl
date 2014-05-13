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
<<<<<<< HEAD
	parentheses_exp, // 0
	array,           // 1
	func_call,       // 2 
	concatenate,     // 3
	math_op,         // 4
	len,             // 5
	assignment,      // 6
	disp_exp,        // 7
	hide_exp,        // 8
	swap,            // 9
	print,           // 10
	print_list,      // 11
	var_decl,        // 12
	var_decl_disp,   // 13
	var_decl_hide,   // 14
	arr_decl,        // 15
	init_list,       // 16
	empty_state,     // 17
	exp_state,       // 18
	declar_state,    // 19
	comp_state,      // 20
	state_list,      // 21
	display_state,   // 22
	select_state,    // 23
	while_state,     // 24
	do_while_state,  // 25
	for_state,       // 26
	jump_continue_state,  // 27
	jump_break_state,     // 28
	jump_ret_state,  // 29
	trans_unit,      // 30
	func_def,        // 31
	para_declar      // 32
=======
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
>>>>>>> eeb3a8c0b1fbd1d5f20dfb9e221997ec6b5ecf0c
} operatorTypeEnum;

typedef enum {
	VOID_TYPE,
	CHAR_TYPE,
	INT_TYPE,
	STRING_TYPE,
	INDEX_TYPE,
	BOOL_TYPE,
<<<<<<< HEAD
	ERROR_TYPE // internal use only
=======
	ERROR_TYPE, // internal use only
	RANGE_TYPE 	// internal use only
>>>>>>> eeb3a8c0b1fbd1d5f20dfb9e221997ec6b5ecf0c
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

varTypeEnum typeChecking(nodeType* root);

void freeTree(nodeType* node);

#endif
