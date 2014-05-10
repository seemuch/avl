#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "syntax_tree.h"

///////////////////////////////////////////////////////////////

nodeType* intConNodeCreator (int value) {
	nodeType *p;

	// allocating memory
	if ((p = (nodeType*)malloc(sizeof(nodeType))) == NULL)
		printf("out of memory\n");

	p->type = INTCON_NODE;
	p->intCon.value = value;

	return p;
}

///////////////////////////////////////////////////////////////

nodeType* charConNodeCreator (char* value) {
	nodeType *p;

	// allocating memory
	if ((p = (nodeType*)malloc(sizeof(nodeType))) == NULL)
		printf("out of memory\n");

	p->type = CHARCON_NODE;
	p->charCon.value = value;

	return p;
}

nodeType* boolConNodeCreator (int value) {
	nodeType *p;

	// allocating memory
	if ((p = (nodeType*)malloc(sizeof(nodeType))) == NULL)
		printf("out of memory\n");

	p->type = BOOLCON_NODE;
	p->boolCon.value = value;

	return p;
}
///////////////////////////////////////////////////////////////

nodeType* strLitNodeCreator (char* value) {
	nodeType* p;

	// allocating memory
	if ((p = (nodeType*)malloc(sizeof(nodeType))) == NULL)
		printf("out of memory\n");
	
	p->type = STRLIT_NODE;
	p->strLit.value = value;

	return p;
}

///////////////////////////////////////////////////////////////

nodeType* varTypeNodeCreator (varTypeEnum type) {
	nodeType* p;

	// allocating memory
	if ((p = (nodeType*)malloc(sizeof(nodeType))) == NULL)
		printf("out of memory\n");
	
	p->type = VARTYPE_NODE;
	p->varType.value = type;

	return p;
}

///////////////////////////////////////////////////////////////

nodeType* idNodeCreator (char* value) {
	nodeType* p;

	// allocating memory
	if ((p = (nodeType*)malloc(sizeof(nodeType))) == NULL)
		printf("out of memory\n");

	p->type = ID_NODE; 
	p->id.value = value;

	return p;
}

///////////////////////////////////////////////////////////////

nodeType* mathOpNodeCreator(char* value) {
	nodeType* p;

	// allocating memroy
	if ((p = (nodeType*)malloc(sizeof(nodeType))) == NULL)
		printf("out of memory\n");

	p->type = MATHOP_NODE; 
	p->mathOp.value = strdup(value);

	return p;
}

///////////////////////////////////////////////////////////////

nodeType* operatorNodeCreator (operatorTypeEnum oprtr, int numOperands, ...) {
	nodeType* p;
	va_list ap;

	// allocating memory
	if ((p = (nodeType*)malloc(sizeof(nodeType))) == NULL)
		printf("out of memory\n");
	
	if ((p->opr.op = (nodeType**)malloc(numOperands * sizeof(nodeType*))) == NULL)
		printf("out of memory\n");
	
	p->type = OPERATOR_NODE;
	p->opr.opType = oprtr;
	p->opr.numOperands = numOperands;

	va_start(ap, numOperands);
	int i;
	for (i = 0; i < numOperands; i ++) {
		p->opr.op[i] = va_arg(ap, nodeType*);
	}
	va_end(ap);

	return p;
}
