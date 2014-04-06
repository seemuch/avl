#ifndef _INCLUDE_H_
#define _INCLUDE_H_

typedef enum {
	intCon,
	strLit,
	varType,
	idType,
	variableType
} nodeTypeEnum;

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

// variables 
typedef struct {
	variableTypeEnum type;
	idNodeType* id;
	int display; // 0 for default; 1 for display; -1 for hide
} variableNodeType;

//////////////////////////////////////////////////////////

typedef struct nodeTypeTag {
	nodeTypeEnum type;

	union {
		intConNodeType intCon;
		strLitNodeType strLit;
		typeSpecNodeType typeSpec;
		varTypeNodeType varType;
		idNodeType id;
		variableNodeType var;
	};

} nodeType;

#endif
