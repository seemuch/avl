typedef enum {  
	type_specifier_node,
	identifier_node,
	constant_node,
	string_literal_node
} nodeEnum;

typedef enum {
	int_type,
	char_type,
	bool_type,
	string_type,
	index_type,
	array_type
} typeEnum;

// type specifier
typedef struct {
	typeEnum typeName;	
} typeSpecNodeType;

// identifiers
typedef struct {
	char* idName;
} identifierNodeType;

// integer constants
typedef struct {
	int value;
} constantNodeType

// string literals
typedef struct {
	char* str;
} strLitNodeType;


/***********************************************/

typedef struct nodeTypeTag {
	nodeEnum type;
	union {
		typeSpecNodeType type_spec;
		identifierNodeType id;
		constantNodeType con;
		strLitNodeType sln;
	};
}nodeType;
