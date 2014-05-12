#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "syntax_tree.h"
#include "st_list.h"
#include "sym_table.h"

void freeTree(nodeType* node);
varTypeEnum typeCheckingOpNode(oprNode* opr);
varTypeEnum typeCheckingSubTree(nodeType* node);

void getParaList(nodeType*);
void clearParaList();

char* idGen(nodeType* node);
varTypeEnum typeGen(nodeType* node);


extern FILE *yyout;
struct identifier** paraList;
size_t paraCount;
int mainFun;

struct st_list symbolTableStack;

// functions related to symbol tables
int countSymbolTable();
void pushNewSymbolTable();
struct identifier* findFunction();
struct identifier* findVariable();

// handler functions 
varTypeEnum parenthesesExpHandler(oprNode* opr);
varTypeEnum arrayHandler(oprNode* opr);
varTypeEnum funcCallHandler(oprNode* opr);
varTypeEnum mathOpHandler(oprNode* opr);
varTypeEnum lenHandler(oprNode* opr);
varTypeEnum assignmentHandler(oprNode* opr);
varTypeEnum dispExpHandler(oprNode* opr);
varTypeEnum hideExpHandler(oprNode* opr);
varTypeEnum swapHandler(oprNode* opr);
varTypeEnum printHandler(oprNode* opr);
varTypeEnum printListHandler(oprNode* opr);
varTypeEnum varDeclHandler(oprNode* opr);
varTypeEnum initListHandler(oprNode* opr);
varTypeEnum expStateHandler(oprNode* opr);
varTypeEnum declarStateHandler(oprNode* opr);
varTypeEnum compStateHandler(oprNode* opr);
varTypeEnum stateListHandler(oprNode* opr);
varTypeEnum selectStateHandler(oprNode* opr);
varTypeEnum whileStateHandler(oprNode* opr);
varTypeEnum doWhileStateHandler(oprNode* opr);
varTypeEnum forStateHandler(oprNode* opr);
varTypeEnum jumpRetStateHandler(oprNode* opr);
varTypeEnum transUnit(oprNode* opr);
varTypeEnum funcDefHandler(oprNode* opr);

varTypeEnum conditionalExpressionHandler(nodeType* node) {
	int i;
	nodeType* temp = 0;
	char* idName = 0;
	struct identifier* id = 0; 	
	varTypeEnum ret = VOID_TYPE;

	temp = node;
	/*
	 * Now temp is a conditional-expression node. 
	 * 		It can be various operator node;
	 * 		It can be ID node;
	 * 		It can be const node;
	 */
	if (temp->type == OPERATOR_NODE) {
		ret = typeCheckingOpNode (&temp->opr);
		if (ret == ERROR_TYPE)
			return ERROR_TYPE;
		return ret;
	}

	else if (temp->type == ID_NODE) {
		idName = idGen(temp);
		id = findVariable(idName);
		if (!id) {
			printf ("Identifier not found: %s\n", idName);
			return ERROR_TYPE;
		}
		return id->type;
	}
	
	// handles const types
	else if (temp->type == INTCON_NODE) {
		return INT_TYPE;
	}

	else if (temp->type == BOOLCON_NODE) {
		return BOOL_TYPE;
	}

	else if (temp->type == CHARCON_NODE) {
		return CHAR_TYPE;
	}

	else if (temp->type == STRLIT_NODE) {
		return STRING_TYPE;
	}
	
	printf("Shining wrong?\n");
	return ERROR_TYPE;
	
}

varTypeEnum typeChecking(nodeType* root) {

	st_list_init(&symbolTableStack);
	
	paraList = NULL;
	paraCount = 0;
	varTypeEnum ret = typeCheckingSubTree(root);
	clearParaList();	

	if (ret != ERROR_TYPE) {
		printf("Type checking succeeds.\n");
	} else {
		printf("Type checking fails.\n");
		freeTree(root);
	}
	return ret;
}



varTypeEnum typeCheckingSubTree(nodeType* node) {
	if (node->type == OPERATOR_NODE) {
		if(typeCheckingOpNode(&node->opr) == ERROR_TYPE) {
			return ERROR_TYPE;
		}
	}

	if (node->type == INTCON_NODE) {
		return INT_TYPE;
	}

	if (node->type == BOOLCON_NODE) {
		return BOOL_TYPE;
	}

	if (node->type == CHARCON_NODE) {
		return CHAR_TYPE;
	}

	if (node->type == STRLIT_NODE) {
		return STRING_TYPE;
	}

	return VOID_TYPE;
}

varTypeEnum typeCheckingOpNode(oprNode* opr) {
	switch (opr->opType) {

		case parentheses_exp:
			if(parenthesesExpHandler(opr) == ERROR_TYPE) return ERROR_TYPE;
			break;

		case array:
			if (arrayHandler(opr) == ERROR_TYPE) return ERROR_TYPE;
			break;

		case func_call:
			if (funcCallHandler(opr) == ERROR_TYPE) return ERROR_TYPE;
			break;

		case math_op:
			if (mathOpHandler(opr) == ERROR_TYPE) return ERROR_TYPE;
			break;

		case len:
			if (lenHandler(opr) == ERROR_TYPE) return ERROR_TYPE;
			break;

		case assignment:
			if (assignmentHandler(opr) == ERROR_TYPE) return ERROR_TYPE;
			break;

		case disp_exp:
			if (dispExpHandler(opr) == ERROR_TYPE) return ERROR_TYPE;
			break;

		case hide_exp:
			if (hideExpHandler(opr) == ERROR_TYPE) return ERROR_TYPE;
			break;

		case swap:
			if (swapHandler(opr) == ERROR_TYPE) return ERROR_TYPE;
			break;

		case print:
			if (printHandler(opr) == ERROR_TYPE) return ERROR_TYPE;
			break;

		case print_list:
			if (printListHandler(opr) == ERROR_TYPE) return ERROR_TYPE;
			break;

		case var_decl:
		case var_decl_disp:
		case var_decl_hide:
			if (varDeclHandler(opr) == ERROR_TYPE) return ERROR_TYPE;
			break;

		case init_list:
			if (initListHandler(opr) == ERROR_TYPE) return ERROR_TYPE;
			break;

		case exp_state:
			if (expStateHandler(opr) == ERROR_TYPE) return ERROR_TYPE;
			break;


		case declar_state:
			if (declarStateHandler(opr) == ERROR_TYPE) return ERROR_TYPE;
			break;

		case comp_state:
			if (compStateHandler(opr) == ERROR_TYPE) return ERROR_TYPE;
			break;

		case state_list:
			if (stateListHandler(opr) == ERROR_TYPE) return ERROR_TYPE;
			break;

		case select_state:
			if (selectStateHandler(opr) == ERROR_TYPE) return ERROR_TYPE;
			break;

		case while_state:
			if (whileStateHandler(opr) == ERROR_TYPE) return ERROR_TYPE;
			break;

		case do_while_state:
			if (doWhileStateHandler(opr) == ERROR_TYPE) return ERROR_TYPE;
			break;

		case for_state:
			if (forStateHandler(opr) == ERROR_TYPE) return ERROR_TYPE;
			break;

		case jump_continue_state:
			break;

		case jump_break_state:
			st_list_del(&symbolTableStack);
			break;

		case jump_ret_state:
			if (jumpRetStateHandler(opr) == ERROR_TYPE) return ERROR_TYPE;
			break;

		case trans_unit:
			if (transUnit(opr) == ERROR_TYPE) return ERROR_TYPE;
			break;

		case func_def:
			if (funcDefHandler(opr) == ERROR_TYPE) return ERROR_TYPE;
			break;

		default:
			break;
	}
	return VOID_TYPE;
}

/*
 * This function returns how many symbol tables are there in the symbol table stack
 * */
int countSymbolTable() {
	int result = 0;
	struct st_node* tmp = symbolTableStack.first;
	if (!tmp)
		return 0;
	while (tmp != symbolTableStack.last) {
		result ++;
		tmp = tmp->next;
	}
	result ++;
	return result;

}

/*
 * This function pushes a new symbol table onto the top of the stack
 */
void pushNewSymbolTable () {
	struct sym_table* newSymbolTable = 0;
	newSymbolTable = (struct sym_table*) malloc(sizeof(struct sym_table));
	sym_table_init(newSymbolTable);
	st_list_add(&symbolTableStack, newSymbolTable);
}

/*
 * This function tries to find a function based on name from the outer most symbol table
 */
struct identifier* findFunction(char* idName) {
	struct identifier* result = sym_table_find(symbolTableStack.last->st, idName);
	return result;
}

/*
 * This function tries to find variables based on name from each symbol table, from top to bottom
 */
struct identifier* findVariable(char* idName) {
	struct identifier* result = st_list_find(&symbolTableStack, idName);
	if (result && result->isFunc == 1)
		return 0;
	return result;
}

varTypeEnum typeGen(nodeType* node) {
	if (node->type != VARTYPE_NODE) 
		return -1;

	return node->varType.value;
}

char* idGen(nodeType* node) {
	if (node->type == ID_NODE)
		return node->id.value;
	return 0;
}

void getParaList(nodeType* node) {
	clearParaList();

	if (node->type != OPERATOR_NODE)
		return ;
	
	paraCount = 1;
	nodeType* tmp = node;
	while (tmp->type == OPERATOR_NODE && tmp->opr.opType == concatenate) {
		paraCount += 1;
		tmp = tmp->opr.op[0];
	}

	tmp = node;
	nodeType ** para_decl = (nodeType **) malloc (paraCount * sizeof (nodeType *)); 
	if (paraCount == 1) {
		para_decl[0] = node;
	}

	else {
		int i =0;
		while (tmp->type == OPERATOR_NODE && tmp->opr.opType == concatenate) {
			para_decl[i] = tmp->opr.op[1];
			i ++;
			tmp = tmp->opr.op[0];
		}
		para_decl[i++] = tmp;
	}

	paraList = (struct identifier**) malloc(paraCount * sizeof(struct identifier*));
	int i = 0;
	for (; i < paraCount; i ++) {
		struct identifier *tmp = (struct identifier*) malloc(sizeof(struct identifier));

		nodeType* nodeTmp = para_decl[i];
		nodeType* temp1 = nodeTmp->opr.op[0];
		nodeType* temp2 = nodeTmp->opr.op[1];

		tmp->type = typeGen(temp1);
		if (temp2->type == ID_NODE) { // an identifier
			tmp->name = idGen(temp2);
			tmp->isArray = 0;
			tmp->isFunc = 0;
			tmp->numArgs = 0;
			tmp->args = 0;
			tmp->argsIsArray = 0;
		}

		else { // an array
			tmp->name = idGen(temp2->opr.op[0]);
			tmp->isArray = 1;
			tmp->isFunc = 0;
			tmp->numArgs = 0;
			tmp->args = 0;
			tmp->argsIsArray = 0;
		}

		paraList[i] = tmp;
	}
	free (para_decl);
	return;
}

void clearParaList() {
	int i = 0;
	if (!paraList) return;

	for (; i < paraCount; i ++) {
		struct identifier* tmp = paraList[i];
		if (!tmp) break;
		if (tmp->args) free(tmp->args);
		if(tmp->argsIsArray) free(tmp->argsIsArray);
		free(tmp);
	}

	if (paraList) { 
		free (paraList); 
		paraList = 0; 
	}

	paraCount = 0;
}

varTypeEnum parenthesesExpHandler(oprNode* opr) {
	return conditionalExpressionHandler(opr->op[0]);
}

varTypeEnum arrayHandler(oprNode* opr) {
	int i;
	nodeType* temp = 0;
	char* idName = 0;
	struct identifier* id = 0;
	varTypeEnum ret = VOID_TYPE;

	// else if (opr->op[0]->type == ID_NODE) {
	idName = idGen(opr->op[0]);
	id = findVariable(idName);
	if (!id) { 
		printf ("Identifier not found: %s\n", idName);
		return ERROR_TYPE;
	}

	varTypeEnum elementType = id->type;

	// one index
	if (opr->numOperands == 2) { 
		temp = opr->op[1];
		ret = conditionalExpressionHandler(temp);

		if (ret == ERROR_TYPE)
			return ERROR_TYPE;

		if (ret != INT_TYPE && ret != INDEX_TYPE)
			return ERROR_TYPE;

		return elementType;
	} 

	// two index, representing a range
	else {
		temp = opr->op[1];
		ret = conditionalExpressionHandler(temp);
		if (ret == ERROR_TYPE)
			return ERROR_TYPE;

		if (ret != INT_TYPE && ret != INDEX_TYPE)
			return ERROR_TYPE;
		
		temp = opr->op[2];
		ret = conditionalExpressionHandler(temp);
		if (ret == ERROR_TYPE)
			return ERROR_TYPE;

		if (ret != INT_TYPE && ret != INDEX_TYPE)
			return ERROR_TYPE;

		return RANGE_TYPE;
	}
}

varTypeEnum funcCallHandler(oprNode* opr) {
	int i;
	nodeType* temp = 0;
	char* idName = 0;
	struct identifier* id = 0;
	varTypeEnum ret = VOID_TYPE;

	idName = idGen(opr->op[0]);

	// no symbol table loaded
	if (!symbolTableStack.first) {
		printf("Function not defined: %s\n.", idName);
		return ERROR_TYPE;
	}
	// id not found
	id = findFunction(idName);
	if (!id) {
		printf("Function not defined: %s\n.", idName);
		return ERROR_TYPE;
	}


	if (!(id->isFunc)) {
		printf("%s is not a function.", idName);
		return ERROR_TYPE;
	}


	// there are arguments
	if (id->numArgs != 0 && opr->numOperands != 2) {
		printf("Function arguments do not match: %s\n", idName);
		return ERROR_TYPE;
	}

	if (opr->numOperands == 2) {
		temp = opr->op[1]; // now temp is an argument expression list

		// count arguments
		int tmp = 1;
		while (temp->type == OPERATOR_NODE && temp->opr.opType == concatenate) {
			tmp += 1;
			temp = temp->opr.op[0];
		}

		// the number of arguments must match
		if (id->numArgs != tmp) {
			printf("Function arguments do not match: %s\n", idName);
			return ERROR_TYPE;
		}

		// argumetn type must match
		temp = opr->op[1];
		while (tmp > 0) {
			if (id->args[tmp - 1] != conditionalExpressionHandler(temp->opr.op[1]))
				return ERROR_TYPE;
			tmp --;
			temp = temp->opr.op[0];
		}

	}
	return VOID_TYPE;
}

varTypeEnum mathOpHandler(oprNode* opr) {
	int i;
	nodeType* temp = 0;
	char* idName = 0;
	struct identifier* id = 0;
	varTypeEnum ret = VOID_TYPE;

	// iterate the node's operands (children)
	for (i=0; i < opr->numOperands; i++) {
		temp = opr->op[i];
		// operator it self
		if (temp->type == MATHOP_NODE) {
			continue;
		}

		// a recursive math_op node
		if (temp->type == OPERATOR_NODE && temp->opr.opType == math_op) {
			if (typeCheckingOpNode(&temp->opr) == ERROR_TYPE)
				return ERROR_TYPE;
		}

		else if (temp->type == OPERATOR_NODE) { // not math_op
			switch (temp->opr.opType) {
				case len:
				case parentheses_exp:
					ret = typeCheckingOpNode(&temp->opr);
					if (ret == ERROR_TYPE)
						return ERROR_TYPE;
					else if (ret != INDEX_TYPE && ret != INT_TYPE && ret != BOOL_TYPE)
						return ERROR_TYPE;
					break;

				case array:
					if (temp->opr.numOperands ==2) {
						ret = typeCheckingOpNode(&temp->opr);
						if (ret == ERROR_TYPE)
							return ERROR_TYPE;
						else if (ret != INT_TYPE && ret != INT_TYPE && ret != BOOL_TYPE)
							return ERROR_TYPE;
						break;
					}
					else {
						printf("Math operation on illegal operands 1.\n");
						return ERROR_TYPE;
					}
					break;

				case func_call:
					idName = idGen(temp->opr.op[0]);
					id = findFunction(idName);
					if (!id) {
						printf("Math operation on illegal operands 2.\n");
						return ERROR_TYPE;
					}
					else if (id->type != INT_TYPE && id->type != INDEX_TYPE && id->type != BOOL_TYPE) {
						printf("Math operation on illegal operands 3.\n");
						return ERROR_TYPE;
					}
					break;

				default:
					printf("Math operation on illegal operands 4.\n");
					return ERROR_TYPE;
			}
		}

		else if (temp->type == ID_NODE) {
			idName = idGen(opr->op[i]);
			id = findVariable(idName);
			if (!id) {
				printf("Identifier not found: %s\n", idName);
				return ERROR_TYPE;
			}

			if (id->type != INT_TYPE && id->type != INDEX_TYPE && id->type != BOOL_TYPE) {
				printf("Math operation can only be applied to int, index or bool type: %s\n", idName);
				return ERROR_TYPE;
			}
		}

		else if (opr->op[i]->type != INTCON_NODE && opr->op[i]->type != BOOLCON_NODE ) {
			printf("Math operation can only be applied to int, index or bool type.\n");
			return ERROR_TYPE;
		}
	}
	return INT_TYPE;
}

varTypeEnum lenHandler(oprNode* opr) {
	int i;
	nodeType* temp = 0;
	char* idName = 0;
	struct identifier* id = 0;
	varTypeEnum ret = VOID_TYPE;

	if (opr->op[0]->type != ID_NODE) {
		printf ("len operator can only be applied to an array.\n");
		return ERROR_TYPE;
	}

	else {
		idName = idGen(opr->op[0]);
		id = findVariable(idName);

		if (!id) {
			printf("Identifier not found: %s\n", idName);
			return ERROR_TYPE;
		}

		if (! (id->isArray) ) {
			printf ("len operator can only be applied to an array.\n");
			return ERROR_TYPE;
		}
	}
	return INT_TYPE;
}

varTypeEnum assignmentHandler(oprNode* opr) {
	int i;
	nodeType* temp = 0;
	char* idName = 0;
	struct identifier* id = 0;
	varTypeEnum ret = VOID_TYPE;

	varTypeEnum beforeEqual = VOID_TYPE;
    varTypeEnum afterEqual = VOID_TYPE;
	int isCollection = 0;

	// deal with the node after assignment first
	temp = opr->op[1];
	/*
	 * Now Temp could be conditional-expression or operator node with init_list opration
	 */
	if (temp->type == OPERATOR_NODE && temp->opr.opType == init_list) {
		isCollection = 1;
		ret = typeCheckingOpNode(&temp->opr);
		if (ret == ERROR_TYPE)
			return ERROR_TYPE;

		afterEqual = ret;
	}

	else {
		ret = conditionalExpressionHandler(temp);
		if (ret == ERROR_TYPE)
			return ERROR_TYPE;
		afterEqual = ret;
	}

	// then deal with the node before the assignemnt operator
	temp = opr->op[0];
	/*
	 * Now temp could be postfix-expression or declarator. 
	 * 1. If it is postfix-expression:
	 * 		It can be an identifier node;
	 * 		It can be an operator node with array operation
	 * 		It CANNOT be anything else, including const node, other kinds of operator nodes
	 *
	 * 2. If it is declarator:
	 * 		It can be an identifier node;
	 * 		It can be an operator node with arr_decl operation, but if so, the first operand is ID. 
	 */

	// It is an operator node, with array operation
	if (temp->type == OPERATOR_NODE && temp->opr.opType == array) {
		if (typeCheckingOpNode(&temp->opr) == ERROR_TYPE)
			return ERROR_TYPE;

		// cannot assign to an array range 
		if (temp->opr.numOperands == 3)
			return ERROR_TYPE;

		// for single array element, get its type
		idName = idGen(temp->opr.op[0]);
		id = findVariable(idName);
		beforeEqual = id->type;

		if (beforeEqual != afterEqual)
			return ERROR_TYPE;
	}


	// It is an operator node, with arr_decl operation
	else if (temp->type == OPERATOR_NODE && temp->opr.opType == arr_decl) {
		temp = temp->opr.op[0];
		idName = idGen(temp);
		id = findVariable(idName);
		if(!id) {
			printf ("Identifier not found: %s\n", idName);
			return ERROR_TYPE;
		}

		if (!id->isArray) {
			printf ("%s is not an array.\n", idName);
			return ERROR_TYPE;
		}

		// for array declaration, what's after the assignment must be a collection
		if (!isCollection)
			return ERROR_TYPE;

		if (id->type != afterEqual)
			return ERROR_TYPE;
	}

	// It is an identifier node
	else if (temp->type == ID_NODE) {
		idName = idGen(temp);
		id = findVariable(idName);
		if(!id) {
			printf ("Identifier not found: %s\n", idName);
			return ERROR_TYPE;
		}
		
		if (id->type != afterEqual)
			return ERROR_TYPE;
	}

	else {
		printf("Illegal assignment.\n");
		return ERROR_TYPE;
	}

	return VOID_TYPE;
}

varTypeEnum dispExpHandler(oprNode* opr) {
	int i;
	nodeType* temp = 0;
	char* idName = 0;
	struct identifier* id = 0;
	varTypeEnum ret = VOID_TYPE;

	temp = opr->op[0];
	idName = idGen(temp);

	id = findVariable(idName);
	if(!id) {
		printf ("Identifier not found: %s\n", idName);
		return ERROR_TYPE;
	}

	if (id->type == STRING_TYPE) {
		printf ("Cannot display string type: %s\n", idName);
		return ERROR_TYPE;
	}

	return VOID_TYPE;
}

varTypeEnum hideExpHandler(oprNode* opr) {
	int i;
	nodeType* temp = 0;
	char* idName = 0;
	struct identifier* id = 0;
	varTypeEnum ret = VOID_TYPE;

	temp = opr->op[0];
	idName = idGen(temp);

	id = findVariable(idName);
	if(!id) {
		printf ("Identifier not found: %s\n", idName);
		return ERROR_TYPE;
	}

	return VOID_TYPE;
}

varTypeEnum swapHandler(oprNode* opr) {
	int i;
	nodeType* temp = 0;
	char* idName = 0;
	struct identifier* id = 0;
	varTypeEnum ret = VOID_TYPE;

	temp = opr->op[0];
	if (temp->type != ID_NODE) {
		printf("Swap can only be applied to an array.\n");
		return ERROR_TYPE;
	}

	idName = idGen(temp);
	id = findVariable(idName);
	if (!id) {
		printf ("Identifier not found: %s\n", idName);
		return ERROR_TYPE;
	}

	if (!id->isArray) {
		printf("Swap can only be applied to an array.\n");
		return ERROR_TYPE;
	}

	// index one
	temp = opr->op[1];
	ret = conditionalExpressionHandler(temp);
	if (ret == ERROR_TYPE)
		return ERROR_TYPE;
	
	if (ret != INT_TYPE && ret != INDEX_TYPE)
		return ERROR_TYPE;

	// index two
	temp = opr->op[2];
	ret = conditionalExpressionHandler(temp);
	if (ret == ERROR_TYPE)
		return ERROR_TYPE;
	
	if (ret != INT_TYPE && ret != INDEX_TYPE)
		return ERROR_TYPE;

	return VOID_TYPE;
}

varTypeEnum printHandler(oprNode* opr) {
	int i;
	nodeType* temp = 0;
	char* idName = 0;
	struct identifier* id = 0;
	varTypeEnum ret = VOID_TYPE;

	if (typeCheckingSubTree(opr->op[0])== ERROR_TYPE)
		return ERROR_TYPE;

	return VOID_TYPE;
}

varTypeEnum printListHandler(oprNode* opr) {
	int i;
	nodeType* temp = 0;
	char* idName = 0;
	struct identifier* id = 0;
	varTypeEnum ret = VOID_TYPE;

	temp = opr->op[0];

	while (temp->type == OPERATOR_NODE && temp->opr.opType == print_list) {
		ret = conditionalExpressionHandler(temp->opr.op[1]);
		if (ret == ERROR_TYPE)
			return ERROR_TYPE;
		temp = temp->opr.op[0];
	}

	ret = conditionalExpressionHandler(temp);
	if (ret == ERROR_TYPE)
		return ERROR_TYPE;

	return VOID_TYPE;
}

varTypeEnum varDeclHandler(oprNode* opr) {
	int i;
	nodeType* temp = 0;
	char* idName = 0;
	struct identifier* id = 0;
	varTypeEnum ret = VOID_TYPE;

	varTypeEnum declaredType = typeGen(opr->op[0]);
	int isArray = 0;

	// whether is an array
	temp = opr->op[1];
	while (temp->type == OPERATOR_NODE) {
		if (temp->opr.opType == arr_decl) break;
		temp = temp->opr.op[0];
	}

	if (temp->opr.opType == arr_decl) { // is array
		isArray = 1;

		if (temp->opr.op[0]->type != ID_NODE) {
			printf("Illegal array declaration!\n");
			return ERROR_TYPE;
		}
		idName = idGen(temp->opr.op[0]);
		id = findVariable(idName);
		if (id) {
			printf("Redefinition of %s\n.", idName);
			return ERROR_TYPE;
		}

		id = (struct identifier*) malloc(sizeof(struct identifier));

		id->name = idName;
		id->type = typeGen(opr->op[0]);
		id->isArray = 1;
		id->isFunc = 0;
		id->numArgs = 0;
		id->args = 0;
		id->argsIsArray = 0;

		sym_table_add(symbolTableStack.first->st, id);
	} 

	else { // not array
		isArray = 0;
		idName = idGen(temp);
		id = findVariable(idName);
		if (id) {
			printf("Redefinition of %s\n", idName);
			return ERROR_TYPE;
		}

		id = (struct identifier*) malloc(sizeof(struct identifier));
		id->name = idName;
		id->type = typeGen(opr->op[0]);
		id->isArray = 0;
		id->isFunc = 0;
		id->numArgs = 0;
		id->args = 0;
		id->argsIsArray = 0;

		sym_table_add(symbolTableStack.first->st, id);

	}

	// assignment
	temp = opr->op[1];
	if (temp->type == OPERATOR_NODE && temp->opr.opType == assignment) {

		temp = temp->opr.op[1];
		if (temp->type == OPERATOR_NODE && temp->opr.opType == init_list) {
			if (!isArray) 
				return ERROR_TYPE;

			ret = typeCheckingOpNode(&temp->opr);

			if (ret == ERROR_TYPE)
				return ERROR_TYPE;

			if (ret != declaredType) 
				return ERROR_TYPE; 
		}

		else {
			if (isArray)
				return ERROR_TYPE;

			ret = conditionalExpressionHandler(temp);
			if (ret == ERROR_TYPE)
				return ERROR_TYPE;
			if ( ret != declaredType)
				return ERROR_TYPE;
		}
	}

	return VOID_TYPE;
}



varTypeEnum initListHandler(oprNode* opr) {
	// TODO
	int i;
	nodeType* temp = 0;
	char* idName = 0;
	struct identifier* id = 0;
	varTypeEnum ret = VOID_TYPE;

	temp = opr->op[0];

	varTypeEnum lastType = ERROR_TYPE;
	if (temp->type == OPERATOR_NODE && temp->opr.opType == concatenate) {
		lastType = conditionalExpressionHandler(temp->opr.op[1]);
		if (ret == ERROR_TYPE)
			return ERROR_TYPE;
	}

	while (temp->type == OPERATOR_NODE && temp->opr.opType == concatenate) {
		ret = conditionalExpressionHandler(temp->opr.op[0]);
		if (ret != lastType)
			return ERROR_TYPE;

		temp = temp->opr.op[0];
	}

	ret = conditionalExpressionHandler(temp);
	if (ret != lastType)
		return ERROR_TYPE;

	return ret;
}

varTypeEnum expStateHandler(oprNode* opr) {
	int i;
	nodeType* temp = 0;
	char* idName = 0;
	struct identifier* id = 0;
	varTypeEnum ret = VOID_TYPE;

	temp = opr->op[0];
	if (temp->type == ID_NODE) {
		idName = idGen(temp->opr.op[1]);
		id = findVariable(idName);
		if (!id) {
			printf("Identifier not found: %s\n", idName);
			return ERROR_TYPE;
		}
	}

	if (temp->type == OPERATOR_NODE) {
		if (typeCheckingOpNode(&temp->opr) == ERROR_TYPE)
			return ERROR_TYPE;
	}

	return VOID_TYPE;
}

varTypeEnum declarStateHandler(oprNode* opr) {
	int i;
	nodeType* temp = 0;
	char* idName = 0;
	struct identifier* id = 0;
	varTypeEnum ret = VOID_TYPE;

	if (typeCheckingSubTree(opr->op[0]) == ERROR_TYPE) 
		return ERROR_TYPE;

	return VOID_TYPE;
}

varTypeEnum compStateHandler(oprNode* opr) {
	int i;
	nodeType* temp = 0;
	char* idName = 0;
	struct identifier* id = 0;
	varTypeEnum ret = VOID_TYPE;

	if (opr->numOperands == 1 && typeCheckingSubTree(opr->op[0]) == ERROR_TYPE) 
		return ERROR_TYPE;
	
	return VOID_TYPE;
}

varTypeEnum stateListHandler(oprNode* opr) {
	int i;
	nodeType* temp = 0;
	char* idName = 0;
	struct identifier* id = 0;
	varTypeEnum ret = VOID_TYPE;

	if (typeCheckingSubTree(opr->op[0]) == ERROR_TYPE) return ERROR_TYPE;
	if (typeCheckingSubTree(opr->op[1]) == ERROR_TYPE) return ERROR_TYPE;

	return VOID_TYPE;
}

varTypeEnum selectStateHandler(oprNode* opr) {
	int i;
	nodeType* temp = 0;
	char* idName = 0;
	struct identifier* id = 0;
	varTypeEnum ret = VOID_TYPE;

	temp = opr->op[0];
	ret = conditionalExpressionHandler(temp);
	if (ret == ERROR_TYPE)
		return ERROR_TYPE;

	if (ret != INT_TYPE && ret != INDEX_TYPE && ret != BOOL_TYPE)
		return ERROR_TYPE;

	pushNewSymbolTable();

	if (typeCheckingSubTree(opr->op[1]) == ERROR_TYPE) return ERROR_TYPE;

	// remove the top symbol table
	st_list_del(&symbolTableStack);

	if (opr->numOperands == 3) {
		pushNewSymbolTable();

		if (typeCheckingSubTree(opr->op[2]) == ERROR_TYPE) return ERROR_TYPE;

		// remove the top symbol table
		st_list_del(&symbolTableStack);
	}

	return VOID_TYPE;
}

varTypeEnum whileStateHandler(oprNode* opr) {
	int i;
	nodeType* temp = 0;
	char* idName = 0;
	struct identifier* id = 0;
	varTypeEnum ret = VOID_TYPE;

	temp = opr->op[0];
	ret = conditionalExpressionHandler(temp);
	if (ret == ERROR_TYPE)
		return ERROR_TYPE;

	if (ret != INT_TYPE && ret != INDEX_TYPE && ret != BOOL_TYPE)
		return ERROR_TYPE;

	pushNewSymbolTable();

	if (typeCheckingSubTree(opr->op[1]) == ERROR_TYPE) return ERROR_TYPE;

	// remove the top symbol table 
	st_list_del(&symbolTableStack);

	return VOID_TYPE;
}

varTypeEnum doWhileStateHandler(oprNode* opr) {
	int i;
	nodeType* temp = 0;
	char* idName = 0;
	struct identifier* id = 0;
	varTypeEnum ret = VOID_TYPE;

	pushNewSymbolTable();

	if (typeCheckingSubTree(opr->op[0]) == ERROR_TYPE) return ERROR_TYPE;

	// remove the top symbol table 
	st_list_del(&symbolTableStack);

	temp = opr->op[1];
	ret = conditionalExpressionHandler(temp);
	if (ret == ERROR_TYPE)
		return ERROR_TYPE;

	if (ret != INT_TYPE && ret != INDEX_TYPE && ret != BOOL_TYPE)
		return ERROR_TYPE;

	return VOID_TYPE;
}

varTypeEnum forStateHandler(oprNode* opr) {
	int i;
	nodeType* temp = 0;
	char* idName = 0;
	struct identifier* id = 0;
	varTypeEnum ret = VOID_TYPE;

	if (opr->op[0]->opr.opType == var_decl ||opr->op[0]->opr.opType == var_decl_disp || opr->op[0]->opr.opType == var_decl_hide) {
		pushNewSymbolTable();

		if (typeCheckingSubTree(opr->op[0]) == ERROR_TYPE) return ERROR_TYPE;

		temp = opr->op[1];
		ret = conditionalExpressionHandler(temp);
		if (ret == ERROR_TYPE)
			return ERROR_TYPE;
		if (ret != INDEX_TYPE && ret != INDEX_TYPE && ret != BOOL_TYPE)
			return ERROR_TYPE;

		if (opr->numOperands == 4 && typeCheckingSubTree(opr->op[2]) == ERROR_TYPE) return ERROR_TYPE;
		if (typeCheckingSubTree(opr->op[opr->numOperands-1]) == ERROR_TYPE) return ERROR_TYPE;

		// remove the top symbol table
		st_list_del(&symbolTableStack);
	} 

	else {
		if (typeCheckingSubTree(opr->op[0]) == ERROR_TYPE) return ERROR_TYPE;

		temp = opr->op[1];
		ret = conditionalExpressionHandler(temp);
		if (ret == ERROR_TYPE)
			return ERROR_TYPE;

		if (ret != INT_TYPE && ret != INDEX_TYPE && ret != BOOL_TYPE)
			return ERROR_TYPE;

		if (opr->numOperands == 4 && typeCheckingSubTree(opr->op[2]) == ERROR_TYPE) return ERROR_TYPE;

		pushNewSymbolTable();

		if (typeCheckingSubTree(opr->op[opr->numOperands-1]) == ERROR_TYPE) return ERROR_TYPE;

		// remove the top symbol table
		st_list_del(&symbolTableStack);
	}

	return VOID_TYPE;
}

varTypeEnum jumpRetStateHandler(oprNode* opr) {
	int i;
	nodeType* temp = 0;
	char* idName = 0;
	struct identifier* id = 0;
	varTypeEnum ret = VOID_TYPE;

	if (opr->numOperands == 1 && typeCheckingSubTree(opr->op[0]) == ERROR_TYPE) return ERROR_TYPE;

	return VOID_TYPE;
}

varTypeEnum transUnit(oprNode* opr) {
	int i;
	nodeType* temp = 0;
	char* idName = 0;
	struct identifier* id = 0;
	varTypeEnum ret = VOID_TYPE;

	if (typeCheckingSubTree(opr->op[0]) == ERROR_TYPE) return ERROR_TYPE;
	if (typeCheckingSubTree(opr->op[1]) == ERROR_TYPE) return ERROR_TYPE;

	return VOID_TYPE;
}

varTypeEnum funcDefHandler(oprNode* opr) {
	int i;
	nodeType* temp = 0;
	char* idName = 0;
	struct identifier* id = 0;
	varTypeEnum ret = VOID_TYPE;

	// if there is not one symbol table in the stack, create one as the base
	if (!symbolTableStack.first) {
		pushNewSymbolTable();
	}


	varTypeEnum returnType = typeGen(opr->op[0]); // function return type
	idName = idGen(opr->op[1]); // function name


	id = findFunction(idName);

	if (id) {
		printf("Function redefinition: %s\n", idName);
		return ERROR_TYPE;
	}

	if (strcmp(opr->op[1]->id.value,"main") == 0)
		mainFun = 1;
	else
		mainFun = 0;

	// clear the parameter list
	clearParaList();
	paraCount = 0;

	// whether main function or not, a new scope is created.	
	pushNewSymbolTable();

	if (mainFun) {
		if(returnType != VOID_TYPE && returnType != INT_TYPE) {
			printf("Main function must return int or void.\n");
			return ERROR_TYPE;
		}

		if (opr->numOperands == 4) {
			printf("Main function must not take any arguments.\n");
			return ERROR_TYPE;
		}

		id = (struct identifier*) malloc(sizeof(struct identifier));
		id->name = idName; 
		id->type = returnType;
		id->isArray = 0;
		id->isFunc = 1;
		id->numArgs = 0;
		id->args = 0;
		id->argsIsArray = 0;

		sym_table_add(symbolTableStack.last->st, id);

	} 
	else if (opr->numOperands == 4) {

		getParaList(opr->op[2]);

		id = (struct identifier*) malloc(sizeof(struct identifier));
		id->name = idName; 
		id->type = returnType;
		id->isArray = 0;
		id->isFunc = 1;
		id->numArgs = paraCount;
		id->args = (varTypeEnum*) malloc(paraCount * sizeof(varTypeEnum));
		id->argsIsArray = (int*) malloc(paraCount * sizeof(int));

		for (int paraIndex = 0; paraIndex < paraCount; paraIndex ++) {
			varTypeEnum test = paraList[paraIndex]->type;
			id->args[paraIndex] = test;
			id->argsIsArray[paraIndex] = paraList[paraIndex]->isArray;
		}

		sym_table_add(symbolTableStack.last->st, id);
	}

	else {
		id = (struct identifier*) malloc(sizeof(struct identifier));
		id->name = idName; 
		id->type = returnType;
		id->isArray = 0;
		id->isFunc = 1;
		id->numArgs = 0;
		id->args = 0;
		id->argsIsArray = 0;

		sym_table_add(symbolTableStack.last->st, id);
	}

	if (typeCheckingSubTree(opr->op[opr->numOperands-1]) == ERROR_TYPE) return ERROR_TYPE;

	// remove the top symbol table
	st_list_del(&symbolTableStack);

	return VOID_TYPE;
}
