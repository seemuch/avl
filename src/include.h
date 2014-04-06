#ifndef _INCLUDE_H_
#define _INCLUDE_H_

#include <string>
#include <vector>
using std::string;
using std::vector;

typedef enum {
	intConType,
	strLitType,
	varType,
	idType,

	operatorType;	
} nodeTypeEnum;

typedef enum {
	variableDeclaration;
} operatorTypeEnum;

typedef enum {
	VOID,
	CHAR,
	INT,
	STRING,
	INDEX,
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
	variableTypeEnum typeType;
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

//////////////////////////////////////////////////////////

// operators
typedef struct {
	operatorTypeEnum opType;
	vector<nodeType*> op;
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
