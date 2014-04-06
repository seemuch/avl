#ifndef _INCLUDE_H_
#define _INCLUDE_H_

#include <string>
#include <vector>
using std::string;
using std::vector;

typedef enum {
	intCon,
	strLit,
	varType,
	idType,
	operatorType;	
} nodeTypeEnum;

typedef enum {
	variableDeclaration;
} operatorTypeEnum;

typedef enum {
	INT,
	CHAR,
	STRING,
	BOOL
} variableTypeEnum;

// integer constants
typedef struct {
	int value;
} intConNodeType;

// string literal
typedef struct {
	string value;
}strLitNodeType;

// type specifier
typedef struct {
	variableTypeEnum;	
} typeSpecNodeType;


// variable types
typedef struct {
	string type;
} varTypeNodeType;

// identifiers
typedef struct {
	string name;
} idNodeType;

// display
typedef struct {
	bool whetheDisplay;
} displayNodeType;

// operators
typedef struct {
	operatorTypeEnum opType;
	vector<nodeType*> op;
} oprNodeType;


/* operators */
typedef struct {
	int oper;
	int nops;
	struct nodeTypeTag **op;
} oprNodeType;


//////////////////////////////////////////////////////////

typedef struct nodeTypeTag {
	nodeTypeEnum type;

	union {
		intConNodeType intCon;
		strLitNodeType strLit;
		typeSpecNodeType typeSpec;
		idNodeType id;

		oprNodeType opr;
	};

} nodeType;

#endif
