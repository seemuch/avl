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
   	DISPLAY,
	OPERATOR;	
} nodeTypeEnum;

typedef enum {
	variableDeclaration;
	func_def
	array_def
	inc_op_post
	dec_op_post
	dec_op_pre
	dec_op_pre
	len
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
        displayNode disp;
		oprNode opr;
	};

} nodeType;

#endif
