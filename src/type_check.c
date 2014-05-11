#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "syntax_tree.h"
#include "st_list.h"
#include "sym_table.h"

void freeTree(nodeType* node);
int typeCheckingOpNode(oprNode* opr);
int typeCheckingSubTree(nodeType* node);

void getParaList(nodeType*);
void clearParaList();

char* idGen(nodeType* node);
varTypeEnum typeGen(nodeType* node);


extern FILE *yyout;
struct identifier** paraList;
size_t paraCount;
int mainFun;

struct st_list symbolTableStack;

int typeChecking(nodeType* root) {

	st_list_init(&symbolTableStack);
	
	paraList = NULL;
	paraCount = 0;
	int ret = typeCheckingSubTree(root);
	clearParaList();	

	if (ret == 0) {
		printf("Type checking succeeds.\n");
	} else {
		printf("Type checking fails.\n");
		freeTree(root);
	}
	return ret;
}



int typeCheckingSubTree(nodeType* node) {
	if (node->type == OPERATOR_NODE) {
		if(typeCheckingOpNode(&node->opr)) 
			return 1;
	}
	return 0;
}

int typeCheckingOpNode(oprNode* opr) {
	int i;
	nodeType* temp = 0;
	char* idName = 0;
	struct identifier* id = 0;
	struct sym_table* newSymbolTable = 0;
	switch (opr->opType) {

		case parentheses_exp:
			temp = opr->op[0];
			/*
			 * Now temp is a conditional-expression node. 
			 * 		It can be various operator node;
			 * 		It can be ID node;
			 * 		It can be const node;
			 */
			if (temp->type == OPERATOR_NODE) {
				if (typeCheckingOpNode (&(temp->opr)))
					return 1;
			}

			else if (temp->type == ID_NODE) {
				idName = idGen(opr->op[0]);
				id = sym_table_find(symbolTableStack.first->st, idName);
				if (!id) {
					printf ("Identifier not found: %s\n", idName);
					return 1;
				}
			}

			break;

		case array:
			// else if (opr->op[0]->type == ID_NODE) {
			idName = idGen(opr->op[0]);
			id = sym_table_find(symbolTableStack.first->st, idName);
			if (!id) { 
				printf ("Identifier not found: %s\n", idName);
				return 1;
			}

			// one index
			if (opr->numOperands == 2) { 
				temp = opr->op[1];
				/*
				 * Now temp is a conditional-expression node. 
				 * 		It can be various operator node;
				 * 		It can be ID node;
				 * 		It can be const node;
				 */
				if (temp->type == OPERATOR_NODE) {
					if (typeCheckingOpNode (&(temp->opr)))
						return 1;
				}

				else if (temp->type == ID_NODE) {
					idName = idGen(opr->op[0]);
					id = sym_table_find(symbolTableStack.first->st, idName);
					if (!id) {
						printf ("Identifier not found: %s\n", idName);
						return 1;
					}
				}
			} 

			// two index, representing a range
			else {
				temp = opr->op[1];
				/*
				 * Now temp is a conditional-expression node. 
				 * 		It can be various operator node;
				 * 		It can be ID node;
				 * 		It can be const node;
				 */
				if (temp->type == OPERATOR_NODE) {
					if (typeCheckingOpNode (&(temp->opr)))
						return 1;
				}

				else if (temp->type == ID_NODE) {
					idName = idGen(opr->op[0]);
					id = sym_table_find(symbolTableStack.first->st, idName);
					if (!id) {
						printf ("Identifier not found: %s\n", idName);
						return 1;
					}
				}

				temp = opr->op[2];
				/*
				 * Now temp is a conditional-expression node. 
				 * 		It can be various operator node;
				 * 		It can be ID node;
				 * 		It can be const node;
				 */
				if (temp->type == OPERATOR_NODE) {
					if (typeCheckingOpNode (&(temp->opr)))
						return 1;
				}

				else if (temp->type == ID_NODE) {
					idName = idGen(opr->op[0]);
					id = sym_table_find(symbolTableStack.first->st, idName);
					if (!id) {
						printf ("Identifier not found: %s\n", idName);
						return 1;
					}
				}

			}

			break;

		case func_call:
			idName = idGen(opr->op[0]);

			// no symbol table loaded
			if (!symbolTableStack.first) {
				printf("Function not defined: %s\n.", idName);
				return 1;
			}

			// id not found
			id = sym_table_find(symbolTableStack.last->st, idName);
			if (!id) {
				printf("Function not defined: %s\n.", idName);
				return 1;
			}

			if (!id->isFunc) {
				printf("%s is not a function.", idName);
				return 1;
			}

			// there are arguments
			if (opr->numOperands == 2) {
				temp = opr->op[1]; // now temp is an argument expression list

				int numArguments = 1;
				while (temp->type == OPERATOR_NODE && temp->opr.opType == concatenate) {
					numArguments += 1;
					temp = temp->opr.op[0];
				}

				if (id->numArgs != numArguments) {
					printf("Function arguments do not match: %s\n", idName);
					return 1;
				}
			}

			break;

		case math_op:
			// iterate the node's operands (children)
			for (i=0; i < opr->numOperands; i++) {
				temp = opr->op[i];
				if (temp->type == MATHOP_NODE)
					continue;

				if (temp->type == OPERATOR_NODE && temp->opr.opType == math_op) {
					if (typeCheckingOpNode(&(temp->opr)))
						return 1;
				}

				else if (temp->type == OPERATOR_NODE) {
					printf("Math operation on illegal operands.\n");
					return 1;
				}

				else if (temp->type == ID_NODE) {
					idName = idGen(opr->op[i]);
					id = sym_table_find(symbolTableStack.first->st, idName);
					if (!id) {
						printf("Identifier not found: %s\n", idName);
						return 1;
					}
					
					if (id->type != INT_TYPE && id->type != INDEX_TYPE && id->type != BOOL_TYPE) {
						printf("Math operation can only be applied to int, index or bool type: %s\n", idName);
						return 1;
					}
				}

				else if (opr->op[i]->type != INTCON_NODE || opr->op[i]->type != BOOLCON_NODE ) {
					printf("Math operation can only be applied to int, index or bool type.\n");
					return 1;
				}
			}
			break;

		case len:
			if (opr->op[0]->type != ID_NODE) {
				printf ("len operator can only be applied to an array.\n");
				return 1;
			}

			else {
				idName = idGen(opr->op[0]);
				id = sym_table_find(symbolTableStack.first->st, idName);

				if (!id) {
					printf("Identifier not found: %s\n", idName);
					return 1;
				}

				if (! (id->isArray) ) {
					printf ("len operator can only be applied to an array.\n");
					return 1;
				}
			}

			break;

		case assignment:
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
				if (typeCheckingOpNode(&(temp->opr))) 
					return 1;
			}

			// It is an operator node, with arr_decl operation
			if (temp->type == OPERATOR_NODE && temp->opr.opType == arr_decl) {
				temp = temp->opr.op[0];
				idName = idGen(temp);
				id = sym_table_find(symbolTableStack.first->st, idName);
				if(!id) {
					printf ("Identifier not found: %s\n", idName);
					return 1;
				}
			}

			// It is an identifier node
			else if (temp->type == ID_NODE) {
				idName = idGen(temp);
				id = sym_table_find(symbolTableStack.first->st, idName);
				if(!id) {
					printf ("Identifier not found: %s\n", idName);
					return 1;
				}
			}

			else {
				printf("Illegal assignment.\n");
				return 1;
			}

			if (typeCheckingSubTree(opr->op[1])) return 1;
			break;

		case disp_exp:
			temp = opr->op[0];
			idName = idGen(temp);

			id = sym_table_find(symbolTableStack.first->st, idName);
			if(!id) {
				printf ("Identifier not found: %s\n", idName);
				return 1;
			}
			
			if (id->type == STRING_TYPE) {
				printf ("Cannot display string type: %s\n", idName);
				return 1;
			}

			break;

		case hide_exp:
			temp = opr->op[0];
			idName = idGen(temp);

			id = sym_table_find(symbolTableStack.first->st, idName);
			if(!id) {
				printf ("Identifier not found: %s\n", idName);
				return 1;
			}
			break;

		case swap:
			temp = opr->op[0];
			if (temp->type != ID_NODE) {
				printf("Swap can only be applied to an array.\n");
				return 1;
			}

			idName = idGen(temp);
			id = sym_table_find(symbolTableStack.first->st, idName);
			if (!id) {
				printf ("Identifier not found: %s\n", idName);
				return 1;
			}

			if (!id->isArray) {
				printf("Swap can only be applied to an array.\n");
				return 1;
			}

			// index one
			temp = opr->op[1];
			if (temp->type == OPERATOR_NODE) {
				if (temp->opr.opType != math_op) {
					printf ("Illegal index.\n");
					return 1;
				}
				if (typeCheckingOpNode(&temp->opr)) return 1;
			}
			else if (temp->type == ID_NODE) {
				idName = idGen(temp);
				id = sym_table_find(symbolTableStack.first->st, idName);
				if (!id) {
					printf("Identifier not found: %s\n", idName);
					return 1;
				}
				if (id->type != INT_TYPE && id->type != INDEX_TYPE) {
					printf("Index can only be int type or index type: %s\n", idName);
					return 1;
				}
			}

			else if (temp->type != INTCON_NODE) {
				printf ("Illegal index.\n");
				return 1;
			}

			// index two
			else if (temp->type != INTCON_NODE ) {
				printf("Index can only be int type or index type: %s\n", idName);
				return 1;
			}

			temp = opr->op[2];
			if (temp->type == OPERATOR_NODE) {
				if (temp->opr.opType != math_op) {
					printf ("Illegal index.\n");
					return 1;
				}
				if (typeCheckingOpNode(&temp->opr)) return 1;
			}

			else if (temp->type == ID_NODE) {
				idName = idGen(temp);
				id = sym_table_find(symbolTableStack.first->st, idName);
				if (!id) {
					printf("Identifier not found: %s\n", idName);
					return 1;
				}
				if (id->type != INT_TYPE && id->type != INDEX_TYPE) {
					printf("Index can only be int or index: %s\n", idName);
					return 1;
				}
			}

			else if (temp->type != INTCON_NODE) {
				printf ("Illegal index.\n");
				return 1;
			}

			break;

		case print:
			if (typeCheckingSubTree(opr->op[0])) return 1;
			break;

		case print_list:
			temp = opr->op[0];
			if (temp->type == OPERATOR_NODE && temp->opr.opType == print_list) {
				if (typeCheckingOpNode(&temp->opr))
					return 1;
			}

			else if (temp->type == OPERATOR_NODE) {
				if (typeCheckingOpNode(&temp->opr))
						return 1;
			}

			else if (temp->type == ID_NODE) {
				idName = idGen(temp);
				id = sym_table_find(symbolTableStack.first->st, idName);
				if(!id) {
					printf ("Identifier not found: %s\n", idName);
					return 1;
				}
			}

			temp = opr->op[2];
			idName = idGen(temp);
			id = sym_table_find(symbolTableStack.first->st, idName);
			if(!id) {
				printf ("Identifier not found: %s\n", idName);
				return 1;
			}

			break;

		case var_decl:
		case var_decl_disp:
		case var_decl_hide:

			// whether is an array
			temp = opr->op[1];
			while (temp->type == OPERATOR_NODE) {
				if (temp->opr.opType == arr_decl) break;
				temp = temp->opr.op[0];
			}

			if (temp->opr.opType == arr_decl) { // is array
				if (temp->opr.op[0]->type != ID_NODE) {
					printf("Illegal array declaration!\n");
					return 1;
				}
				idName = idGen(temp->opr.op[0]);
				id = sym_table_find(symbolTableStack.first->st, idName);
				if (id) {
					printf("Redefinition of %s\n.", idName);
					return 1;
				}

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
				idName = idGen(temp);
				id = sym_table_find(symbolTableStack.first->st, idName);
				if (id) {
					printf("Redefinition of %s\n", idName);
					return 1;
				}
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
			if (opr->op[1]->type == OPERATOR_NODE && opr->op[1]->opr.opType == assignment) {
				temp = opr->op[1]->opr.op[1];
				if (temp->type == ID_NODE) {
					idName = idGen(temp);
					id = sym_table_find(symbolTableStack.first->st, idName);
					if (!id) {
						printf("Identifier not found: %s\n", idName);
						return 1;
					}
				}
				else if (temp->type == OPERATOR_NODE) {
					if (typeCheckingOpNode(&temp->opr))
							return 1;
				}
			}
			break;

		case init_list:
			temp = opr->op[0];
			if (temp->type == ID_NODE) {
				idName = idGen(temp);
				id = sym_table_find(symbolTableStack.first->st, idName);
				if (!id) {
					printf("Identifier not found: %s\n", idName);
					return 1;
				}
			}

			else if (temp->type == OPERATOR_NODE && temp->opr.opType == concatenate) {
				idName = idGen(temp->opr.op[1]);
				id = sym_table_find(symbolTableStack.first->st, idName);
				if (!id) {
					printf("Identifier not found: %s\n", idName);
					return 1;
				}

				if (typeCheckingOpNode(&(temp->opr.op[0]->opr)))
					return 1;
			}

			if (typeCheckingSubTree(opr->op[0])) return 1;
			break;

		case exp_state:
			temp = opr->op[0];
			if (temp->type == ID_NODE) {
				idName = idGen(temp->opr.op[1]);
				id = sym_table_find(symbolTableStack.first->st, idName);
				if (!id) {
					printf("Identifier not found: %s\n", idName);
					return 1;
				}
			}

			if (temp->type == OPERATOR_NODE) {
				if (typeCheckingOpNode(&temp->opr))
					return 1;
			}

			break;

		case declar_state:
			if (typeCheckingSubTree(opr->op[0])) return 1;
			break;

		case comp_state:
			if (opr->numOperands == 1 && typeCheckingSubTree(opr->op[0])) return 1;
			break;

		case state_list:
			if (typeCheckingSubTree(opr->op[0])) return 1;
			if (typeCheckingSubTree(opr->op[1])) return 1;
			break;

		case select_state:
			temp = opr->op[0];
			if (temp->type == OPERATOR_NODE) {
				if (temp->opr.opType != math_op) {
					printf ("Illegal index.\n");
					return 1;
				}
				if (typeCheckingOpNode(&temp->opr)) return 1;

			}

			else if (temp->type == ID_NODE) {
				idName = idGen(temp);
				id = sym_table_find(symbolTableStack.first->st, idName);
				if (!id) {
					printf ("Identifier not found: %s\n", idName);
					return 1;
				}
				if (id->type != BOOL_TYPE && id->type != INT_TYPE) {
					printf("Illegal if condition\n");
					return 1;
				}
			}

			else if (temp->type != INTCON_NODE && temp->type != BOOLCON_NODE) {
				printf("Illegal if condition\n");
				return 1;
			}
			
			// generate new symbol table for the new scope
			newSymbolTable = (struct sym_table*) malloc(sizeof(struct sym_table));
			sym_table_init(newSymbolTable);
			st_list_add(&symbolTableStack, newSymbolTable);

			if (typeCheckingSubTree(opr->op[1])) return 1;

			// remove the top symbol table
			st_list_del(&symbolTableStack);

			if (opr->numOperands == 3) {
				// generate new symbol table for the new scope
                newSymbolTable = (struct sym_table*) malloc(sizeof(struct sym_table));
				sym_table_init(newSymbolTable);
                st_list_add(&symbolTableStack, newSymbolTable);

				if (typeCheckingSubTree(opr->op[2])) return 1;

				// remove the top symbol table
                st_list_del(&symbolTableStack);
			}
			break;

		case while_state:
			temp = opr->op[0];
			if (temp->type == OPERATOR_NODE) {
				if (temp->opr.opType != math_op) {
					printf ("Illegal index.\n");
					return 1;
				}
				if (typeCheckingOpNode(&temp->opr)) return 1;

			}

			else if (temp->type == ID_NODE) {
				idName = idGen(temp);
				id = sym_table_find(symbolTableStack.first->st, idName);
				if (!id) {
					printf ("Identifier not found: %s\n", idName);
					return 1;
				}
				if (id->type != BOOL_TYPE && id->type != INT_TYPE) {
					printf("Illegal while condition\n");
					return 1;
				}
			}

			else if (temp->type != INTCON_NODE && temp->type != BOOLCON_NODE) {
				printf("Illegal while condition\n");
				return 1;
			}

			// generate new symbol table for the new scope
			newSymbolTable = (struct sym_table*) malloc(sizeof(struct sym_table));
			sym_table_init(newSymbolTable);
			st_list_add(&symbolTableStack, newSymbolTable);

			if (typeCheckingSubTree(opr->op[1])) return 1;

			// remove the top symbol table 
			st_list_del(&symbolTableStack);
			
			break;

		case do_while_state:
			// generate new symbol table for the new scope
			newSymbolTable = (struct sym_table*) malloc(sizeof(struct sym_table));
			sym_table_init(newSymbolTable);
			st_list_add(&symbolTableStack, newSymbolTable);

			if (typeCheckingSubTree(opr->op[0])) return 1;

			// remove the top symbol table 
			st_list_del(&symbolTableStack);
			
			temp = opr->op[1];
			if (temp->type == OPERATOR_NODE) {
				if (temp->opr.opType != math_op) {
					printf ("Illegal index.\n");
					return 1;
				}
				if (typeCheckingOpNode(&temp->opr)) return 1;

			}

			else if (temp->type == ID_NODE) {
				idName = idGen(temp);
				id = sym_table_find(symbolTableStack.first->st, idName);
				if (!id) {
					printf ("Identifier not found: %s\n", idName);
					return 1;
				}
				if (id->type != BOOL_TYPE && id->type != INT_TYPE) {
					printf("Illegal while condition\n");
					return 1;
				}
			}

			else if (temp->type != INTCON_NODE && temp->type != BOOLCON_NODE) {
				printf("Illegal while condition\n");
				return 1;
			}

			break;

		case for_state:
			if (opr->op[0]->opr.opType == var_decl ||opr->op[0]->opr.opType == var_decl_disp || opr->op[0]->opr.opType == var_decl_hide) {
				// generate new symbol table for the new scope
                newSymbolTable = (struct sym_table*) malloc(sizeof(struct sym_table));
				sym_table_init(newSymbolTable);
                st_list_add(&symbolTableStack, newSymbolTable);

				if (typeCheckingSubTree(opr->op[0])) return 1;

				temp = opr->op[1];
				if (temp->type == OPERATOR_NODE) {
					if (temp->opr.opType != math_op) {
						printf ("Illegal index.\n");
						return 1;
					}
					if (typeCheckingOpNode(&temp->opr)) return 1;

				}

				else if (temp->type == ID_NODE) {
					idName = idGen(temp);
					id = sym_table_find(symbolTableStack.first->st, idName);
					if (!id) {
						printf ("Identifier not found: %s\n", idName);
						return 1;
					}
					if (id->type != BOOL_TYPE && id->type != INT_TYPE) {
						printf("Illegal for condition\n");
						return 1;
					}
				}

				else if (temp->type != INTCON_NODE && temp->type != BOOLCON_NODE) {
					printf("Illegal for condition\n");
					return 1;
				}

				if (opr->numOperands == 4 && typeCheckingSubTree(opr->op[2])) return 1;
				if (typeCheckingSubTree(opr->op[opr->numOperands-1])) return 1;

				// remove the top symbol table
                st_list_del(&symbolTableStack);
			} 

			else {
				if (typeCheckingSubTree(opr->op[0])) return 1;

				temp = opr->op[1];
				if (temp->type == OPERATOR_NODE) {
					if (temp->opr.opType != math_op) {
						printf ("Illegal index.\n");
						return 1;
					}
					if (typeCheckingOpNode(&temp->opr)) return 1;

				}

				else if (temp->type == ID_NODE) {
					idName = idGen(temp);
					id = sym_table_find(symbolTableStack.first->st, idName);
					if (!id) {
						printf ("Identifier not found: %s\n", idName);
						return 1;
					}
					if (id->type != BOOL_TYPE && id->type != INT_TYPE) {
						printf("Illegal for condition\n");
						return 1;
					}
				}

				else if (temp->type != INTCON_NODE && temp->type != BOOLCON_NODE) {
					printf("Illegal for condition\n");
					return 1;
				}

				if (opr->numOperands == 4 && typeCheckingSubTree(opr->op[2])) return 1;

				// generate new symbol table for the new scope
                newSymbolTable = (struct sym_table*) malloc(sizeof(struct sym_table));
				sym_table_init(newSymbolTable);
                st_list_add(&symbolTableStack, newSymbolTable);

				if (typeCheckingSubTree(opr->op[opr->numOperands-1])) return 1;

				// remove the top symbol table
                st_list_del(&symbolTableStack);
			}
			break;

		case jump_continue_state:
			break;

		case jump_break_state:
			st_list_del(&symbolTableStack);
			break;

		case jump_ret_state:
			if (opr->numOperands == 1 && typeCheckingSubTree(opr->op[0])) return 1;
			st_list_del(&symbolTableStack);
			break;

		case trans_unit:
			if (typeCheckingSubTree(opr->op[0])) return 1;
			if (typeCheckingSubTree(opr->op[1])) return 1;
			break;

		case func_def:
			// if there is not one symbol table in the stack, create one as the base
			if (!symbolTableStack.first) {
				// generate new symbol table for the new scope
				newSymbolTable = (struct sym_table*) malloc(sizeof(struct sym_table));
				sym_table_init(newSymbolTable);
				st_list_add(&symbolTableStack, newSymbolTable);
			}

			varTypeEnum returnType = typeGen(opr->op[0]); // function return type
			idName = idGen(opr->op[1]); // function name

			id = sym_table_find(symbolTableStack.last->st, idName);
			if (id) {
				printf("Function redefinition: %s\n", idName);
				return 1;
			}

			if (strcmp(opr->op[1]->id.value,"main") == 0)
				mainFun = 1;
			else
				mainFun = 0;

			// clear the parameter list
			clearParaList();
			paraCount = 0;

			// whether main function or not, a new scope is created.	
			// generate new symbol table for the new scope
			newSymbolTable = (struct sym_table*) malloc(sizeof(struct sym_table));
			sym_table_init(newSymbolTable);
			st_list_add(&symbolTableStack, newSymbolTable);

			if (mainFun) {
				if(returnType != VOID_TYPE && returnType != INT_TYPE) {
					printf("Main function must return int or void.\n");
					return 1;
				}

				if (opr->numOperands == 4) {
					printf("Main function must not take any arguments.\n");
					return 1;
				}

				id->name = idName; 
				id->type = returnType;
				id->isArray = 0;
				id->isFunc = 1;
				id->numArgs = 0;
				id->args = 0;
				id->argsIsArray = 0;

				sym_table_add(symbolTableStack.first->st, id);

			} 
			else if (opr->numOperands == 4) {
				getParaList(opr->op[2]);

				id->name = idName; 
				id->type = returnType;
				id->isArray = 0;
				id->isFunc = 0;
				id->numArgs = paraCount;
				id->args = (varTypeEnum*) malloc(sizeof(varTypeEnum));
				id->argsIsArray = (int*) malloc(sizeof(varTypeEnum));
				for (int paraIndex = 0; paraIndex < paraCount; paraIndex ++) {
					id->args[i] = paraList[i]->type;
					id->argsIsArray[i] = paraList[i]->isArray;
				}

				sym_table_add(symbolTableStack.first->st, id);
			}

			if (typeCheckingSubTree(opr->op[opr->numOperands-1])) return 1;

			// remove the top symbol table
            st_list_del(&symbolTableStack);
			break;

		default:
			break;
	}
	return 0;
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
	free (paraList);
}

varTypeEnum typeGen(nodeType* node) {
	if (node->type != VARTYPE_NODE) 
		return -1;

	switch (node->varType.value) {
		case VOID_TYPE:
			return VOID_TYPE;
			break;
		case CHAR_TYPE:
			return CHAR_TYPE;
			break;
		case INT_TYPE:
			return INT_TYPE;
			break;
		case STRING_TYPE:
			return STRING_TYPE;
			break;
		case INDEX_TYPE:
			return INDEX_TYPE;
			break;
		case BOOL_TYPE:
			return BOOL_TYPE;
			break;
	}
}

char* idGen(nodeType* node) {
	if (node->type == ID_NODE)
		return node->id.value;
	else 
		printf ("Shining");
	return 0;
}

int getNumPara(oprNode* node) {
	if (node->opType == concatenate)
		return getNumPara(&node->op[0]->opr) + 1;
	return 1;
}

void getParaList(nodeType* node) {
	if (node->type != OPERATOR_NODE)
		return ;
	paraCount = getNumPara(&node->opr);

	nodeType* tmp = node;
	nodeType ** para_decl = (nodeType **) malloc (paraCount * sizeof (nodeType *)); 
	if (paraCount == 1) {
		para_decl[0] = node;
	}

	else {
		int i =0;
		while (tmp->opr.opType == concatenate) {
			para_decl[i] = tmp->opr.op[1];
			i ++;
			tmp = tmp->opr.op[0];
		}
	}

	clearParaList();
	paraList = (struct identifier**) malloc(paraCount * sizeof(struct identifier*));
	for (int i = 0; i < paraCount; i ++) {
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

		free (nodeTmp);

		paraList[i] = tmp;
	}
	free (para_decl);
}

