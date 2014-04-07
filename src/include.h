#ifndef _INCLUDE_H_
#define _INCLUDE_H_

#include <string>
#include <vector>
using std::string;
using std::vector;

typedef enum {
	INTCON,
	STRLIT,
	VARTYPE,
	ID,
	UNARY,
   	DISPLAY,
	OPERATOR
} nodeTypeEnum;

typedef enum {
	func_def,
	array_def,
	inc_op_post,
	dec_op_post,
	dec_op_pre,
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
    fun_def,
    para_declar
} operatorTypeEnum;

typedef enum {
	VOID,
	CHAR,
	INT,
	STRING,
	INDEX,
	BOOL
} varTypeEnum;

// integer constants
typedef struct {
	int value;
} intConNode;

// string literal
typedef struct {
	string value;
} strLitNode;

// type specifier
typedef struct {
	varTypeEnum value;	
} varTypeNode;

// identifiers
typedef struct {
	string value;
} idNode;

// display
typedef struct {
	bool value;
} displayNode;

// unary_operator, + - !
typedef struct {
	char value;
} unaryNode

//////////////////////////////////////////////////////////

// operators
typedef struct {
	operatorTypeEnum opType;
	vector<nodeType*> op;
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
