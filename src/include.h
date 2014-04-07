#ifndef _INCLUDE_H_
#define _INCLUDE_H_

typedef enum {
	INTCON_NODE,
	STRLIT_NODE,
	VARTYPE_NODE,
	ID_NODE,
	UNARY_NODE,
   	DISPLAY_NODE,
	OPERATOR_NODE
} nodeTypeEnum;

typedef enum {
	variableDeclaration,
	func_call,
	array,
	arrayDeclaration,
	inc_op_post,
	dec_op_post,
	inc_op_pre,
	dec_op_pre,
	len,
	concatenate,	
	exp_state,
	declar_state,
	comp_state,
	state_list,
	display_state,
	select_state,
	iter_state,
	jump_continue_state,
	jump_break_state,
	jump_ret_state,
	assignment,	
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

// display
typedef struct {
	int value;
} displayNode;

// unary_operator, + - !
typedef struct {
	char value;
} unaryNode;

//////////////////////////////////////////////////////////

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
		strLitNode strLit;
		varTypeNode varType;
		idNode id;
		unaryNode unary;
        displayNode disp;
		oprNode opr;
		
	};

} nodeType;

#endif
