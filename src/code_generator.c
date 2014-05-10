#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "syntax_tree.h"
#include "st_list.h"
#include "sym_table.h"

void freeTree(nodeType* node);
int generateOpNode(oprNode* opr);
int generateSubtree(nodeType* node);
void print_append(char* s, int space);
void print_append_int(int value, int space);
void print_indent();
void print_header();
void print_line(const char *line);
void avl_code_generator(nodeType* root);

int newLine = 1;
int indent = 0;
extern FILE *yyout;
char** paraList;
int paraCount;
int mainFun;

void avl_code_generator(nodeType* root) {
	printf("begin translate.\n");
	/*sym_table_init(&funTable);
	if (buildFunTable(&funTable, &root->opr)) {
		printf("Fail\n");
		return;
	}
	*/
	print_header();
	paraList = NULL;
	int ret = generateSubtree(root);
	freeTree(root);
	//sym_table_destroy(&funTable);
	if (ret == 0) {
		printf("Succeed\n");
	} else {
		printf("Fail\n");
	}
}

int numPara(oprNode* node) {
	if (node->opType == concatenate)
		return numPara(&node->op[0]->opr) + 1;
	return 1;
}


int generateSubtree(nodeType* node) {
	switch (node->type) {
		case INTCON_NODE:
			print_append_int(node->intCon.value, 1);
			break;
		case CHARCON_NODE:
			print_append(node->charCon.value, 1);
			break;
		case STRLIT_NODE:
			print_append(node->strLit.value, 1);
			break;
		case VARTYPE_NODE:
			switch (node->varType.value) {
				case VOID_TYPE:
					print_append("void", 1);
					break;
				case CHAR_TYPE:
					print_append("AvlChar", 1);
					break;
				case INT_TYPE:
					print_append("AvlInt", 1);
					break;
				case STRING_TYPE:
					print_append("string", 1);
					break;
				case INDEX_TYPE:
					print_append("AvlIndex", 1);
					break;
				case BOOL_TYPE:
					print_append("AvlBool", 1);
					break;
			}
			break;
		case ID_NODE:
			print_append(node->id.value, 1);
			break;
		case MATHOP_NODE:
			print_append(node->mathOp.value, 1);
			break;
		case OPERATOR_NODE:
			if(generateOpNode(&node->opr)) return 1;
			break;
	}
	return 0;
}

int generateOpNode(oprNode* opr) {
	int i;
	char* id;
	nodeType* temp;
	switch (opr->opType) {
		case parentheses_exp:
			print_append("(", 1);
			if (generateSubtree(opr->op[0])) return 1;
			print_append(")", 1);
			break;
		case array:
			if (generateSubtree(opr->op[0])) return 1;
			if (opr->numOperands == 2) {
				print_append("[", 0);
				if (generateSubtree(opr->op[1])) return 1;
				print_append("]", 1);
			} else {
				print_append(".subarray(", 0);
				if (generateSubtree(opr->op[1])) return 1;
				print_append(",", 0);
				if (generateSubtree(opr->op[2])) return 1;
				print_append(")", 1);
			}
			break;
		case func_call:
			if (generateSubtree(opr->op[0])) return 1;
			print_append("(", 0);
			if (opr->numOperands == 2 && generateSubtree(opr->op[1])) return 1;
			print_append(")", 1);
			break;
		case concatenate:	
			if (generateSubtree(opr->op[0])) return 1;
			print_append(",", 0);
			if (generateSubtree(opr->op[1])) return 1;
			break;
		case math_op:
			for (i=0; i<opr->numOperands; i++) {
				if (generateSubtree(opr->op[i])) return 1;
			}
			break;
		case len:
			if (generateSubtree(opr->op[0])) return 1;
			print_append(".size()", 0);
			break;
		case assignment:	
			if (generateSubtree(opr->op[0])) return 1;
			print_append("=", 1);
			if (generateSubtree(opr->op[1])) return 1;
			break;
		case disp_exp:
			break;
		case hide_exp:
			break;
		case swap:
			if (generateSubtree(opr->op[0])) return 1;
			print_append(".swap(", 0);
			if (generateSubtree(opr->op[1])) return 1;
			print_append(",", 0);
			if (generateSubtree(opr->op[2])) return 1;
			print_append(")", 1);
			break;
		case print:
			print_append("std::cout <<", 1);
			if (generateSubtree(opr->op[0])) return 1;
			print_append("<< std::endl", 1);
			break;
		case print_list:
			if (generateSubtree(opr->op[0])) return 1;
			if (opr->numOperands == 2) {
				print_append("<<", 1);
				if (generateSubtree(opr->op[1])) return 1;
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
				print_append("AvlArray<", 1);
				if (generateSubtree(opr->op[0])) return 1;
				print_append(">", 0);
				if (generateSubtree(temp->opr.op[0])) return 1;
				if (temp->opr.numOperands == 2) {
					print_append("(", 0);
					if (generateSubtree(temp->opr.op[1])) return 1;
					print_append(")", 0);
				}
			} else { // not array
				if (generateSubtree(opr->op[0])) return 1;
				if (generateSubtree(temp)) return 1;
			}
			// assignment
			if (opr->op[1]->type == OPERATOR_NODE && opr->op[1]->opr.opType == assignment) {
				print_append("=", 1);
				if (generateSubtree(opr->op[1]->opr.op[1])) return 1;
			}
			print_append(";", 0);
			newLine = 1;
			// id to temp
			while (temp->type == OPERATOR_NODE) {
				temp = temp->opr.op[0];
			}
			id = temp->id.value;
			print_indent();
			fprintf(yyout, "%s.set_name(\"%s\");\n", id, id);
			if (opr->opType == var_decl_disp) {
				print_indent();
				fprintf(yyout, "__avl__vi->addObject(&%s, \"%s\");\n",id, id);
			}
			newLine = 1;
			break;
		case arr_decl:
			if (generateSubtree(opr->op[0])) return 1;
			break;
		case init_list:
			print_append("{", 1);
			if (generateSubtree(opr->op[0])) return 1;
			print_append("}", 1);
			break;
		case exp_state:
			if (generateSubtree(opr->op[0])) return 1;
			print_append(";", 0);
			newLine = 1;
			break;
		case declar_state:
			if (generateSubtree(opr->op[0])) return 1;
			break;
		case comp_state:
			print_append("{", 1);
			newLine = 1;
			indent ++;
			if (opr->numOperands == 1 && generateSubtree(opr->op[0])) return 1;
			indent --;
			print_append("}", 0);
			newLine = 1;
			break;
		case state_list:
			if (generateSubtree(opr->op[0])) return 1;
			if (generateSubtree(opr->op[1])) return 1;
			break;
		case display_state:
			print_append("__avl__vi->start();", 1);
			newLine = 1;
			print_append("avlSleep(0.5);", 0);
			newLine = 1;
			if (generateSubtree(opr->op[0])) return 1;
			print_append("avlSleep(0.1);", 1);
			newLine = 1;
			print_append("__avl__vi->stop();", 0);
			newLine = 1;
			break;
		case select_state:
			print_append("if (", 1);
			if (generateSubtree(opr->op[0])) return 1;
			print_append(")", 1);
			if (opr->op[1]->type == OPERATOR_NODE && opr->op[1]->opr.opType != comp_state) {
				print_append("{", 1);
				newLine = 1;
				indent ++;
				if (generateSubtree(opr->op[1])) return 1;
				indent --;
				print_append("}", 0);
				newLine = 1;
			} else {
				if (generateSubtree(opr->op[1])) return 1;
			}
			if (opr->numOperands == 3) {
				print_append("else", 1);
				if (opr->op[2]->type == OPERATOR_NODE && opr->op[2]->opr.opType != comp_state) {
					print_append("{", 1);
					newLine = 1;
					indent ++;
					if (generateSubtree(opr->op[2])) return 1;
					indent --;
					print_append("}", 0);
					newLine = 1;
				} else {
					if (generateSubtree(opr->op[2])) return 1;
				}
			}
			break;
		case while_state:
			print_append("while (", 1);
			if (generateSubtree(opr->op[0])) return 1;
			print_append(")", 1);
			if (opr->op[1]->type == OPERATOR_NODE && opr->op[1]->opr.opType != comp_state) {
				print_append("{", 1);
				newLine = 1;
				indent ++;
				if (generateSubtree(opr->op[1])) return 1;
				indent --;
				print_append("}", 0);
				newLine = 1;
			} else {
				if (generateSubtree(opr->op[1])) return 1;
			}
			break;
		case do_while_state:
			print_append("do", 1);
			if (opr->op[0]->type == OPERATOR_NODE && opr->op[0]->opr.opType != comp_state) {
				print_append("{", 1);
				newLine = 1;
				indent ++;
				if (generateSubtree(opr->op[0])) return 1;
				indent --;
				print_append("}", 0);
				newLine = 1;
			} else {
				if (generateSubtree(opr->op[0])) return 1;
			}
			print_append("while (", 1);
			if (generateSubtree(opr->op[1])) return 1;
			print_append(");", 1);
			newLine = 1;
			break;
		case for_state:
			if (opr->op[0]->opr.opType == var_decl ||opr->op[0]->opr.opType == var_decl_disp || opr->op[0]->opr.opType == var_decl_hide) {
				print_append("{", 0);
				newLine = 1;
				indent ++;
				if (generateSubtree(opr->op[0])) return 1;
				print_append("for (", 1);
				print_append(";", 0);
				if (generateSubtree(opr->op[1])) return 1;
				print_append(";", 0);
				if (opr->numOperands == 4 && generateSubtree(opr->op[2])) return 1;
				print_append(")", 1);
				if (opr->op[opr->numOperands-1]->type == OPERATOR_NODE && opr->op[opr->numOperands-1]->opr.opType != comp_state) {
					print_append("{", 1);
					newLine = 1;
					indent ++;
					if (generateSubtree(opr->op[opr->numOperands-1])) return 1;
					indent --;
					print_append("}", 0);
					newLine = 1;
				} else {
					if (generateSubtree(opr->op[opr->numOperands-1])) return 1;
				}
				indent --;
				print_append("}", 0);
				newLine = 1;
			} else {
				print_append("for (", 1);
				if (generateSubtree(opr->op[0])) return 1;
				print_append(";", 0);
				if (generateSubtree(opr->op[1])) return 1;
				print_append(";", 0);
				if (opr->numOperands == 4 && generateSubtree(opr->op[2])) return 1;
				print_append(")", 1);
				if (opr->op[opr->numOperands-1]->type == OPERATOR_NODE && opr->op[opr->numOperands-1]->opr.opType != comp_state) {
					print_append("{", 1);
					newLine = 1;
					indent ++;
					if (generateSubtree(opr->op[opr->numOperands-1])) return 1;
					indent --;
					print_append("}", 0);
					newLine = 1;
				} else {
					if (generateSubtree(opr->op[opr->numOperands-1])) return 1;
				}
			}
			break;
		case jump_continue_state:
			print_append("continue;", 1);
			newLine = 1;
			break;
		case jump_break_state:
			print_append("break;", 1);
			newLine = 1;
			break;
		case jump_ret_state:
			if (mainFun) {
				print_append("__avl__loop.join();", 1);
				newLine = 1;
				print_append("delete __avl__vi;", 1);
				newLine = 1;
			} else {
				for (i=0; i<paraCount; i++) {
					print_append(paraList[i], 0);
					print_append(".lowlight();", 0);
					newLine = 1;
				}
				print_append("__avl__vi->restore();", 1);
				newLine = 1;
			}
			print_append("return", 1);
			if (opr->numOperands == 1 && generateSubtree(opr->op[0])) return 1;
			print_append(";", 0);
			newLine = 1;
			break;
		case trans_unit:
			if (generateSubtree(opr->op[0])) return 1;
			if (generateSubtree(opr->op[1])) return 1;
			break;
		case func_def:
			if (generateSubtree(opr->op[0])) return 1;
			if (generateSubtree(opr->op[1])) return 1;
			if (strcmp(opr->op[1]->id.value,"main") == 0)
				mainFun = 1;
			else
				mainFun = 0;
			print_append("(", 0);
			if (paraList) free(paraList);
			paraCount = 0;
			if (mainFun) {
				print_append("int argc, char *argv[]",0);
			} else if (opr->numOperands == 4) {
				paraList = (char**)malloc(sizeof(char*)*numPara(&opr->op[2]->opr));
				if (generateSubtree(opr->op[2])) return 1;
			}
			print_append(")", 1);
			newLine = 1;
			print_append("{",0);
			newLine = 1;
			indent ++;
			if (mainFun) {
				print_line("\tstd::thread __avl__loop(__avl__display, argc, argv);");
				print_line("\t__avl__mtx.lock();");
				print_line("\t__avl__cv.wait(__avl__mtx, __avl__ready);");
				print_line("\t__avl__mtx.unlock();");
				print_line("\tavlSleep(0.5);");
			} else {
				print_line("\t__avl__vi->reset();");
				for (i=0; i<paraCount; i++) {
					print_append(paraList[i], 0);
					print_append(".lowlight();", 0);
					newLine = 1;
					print_append(paraList[i], 0);
					print_append(".highlight();", 0);
					newLine = 1;
				}
			}		
			if (generateSubtree(opr->op[opr->numOperands-1])) return 1;
			if (opr->op[0]->varType.value == VOID_TYPE) {
				if (mainFun) {
					print_line("\t__avl__loop.join();");
					print_line("\tdelete __avl__vi;");
				} else {
					for (i=0; i<paraCount; i++) {
						print_append(paraList[i], 0);
						print_append(".lowlight();", 0);
						newLine = 1;
					}
					print_line("\t__avl__vi->restore();");
				}
			}
			indent --;
			newLine = 1;
			print_append("}",0);
			break;
		case para_declar:
			//array deaclar
			if (opr->op[0]->varType.value == INT_TYPE && opr->op[1]->type == OPERATOR_NODE && 
					opr->op[1]->opr.opType == arr_decl) {
				print_append("AvlArray<AVlInt>", 1);
				temp = opr->op[1];
				while (temp->type != ID_NODE) temp = temp->opr.op[0];
				paraList[paraCount] = temp->id.value;
				paraCount ++;
				if (generateSubtree(opr->op[1])) return 1;
			} else { 
				if (generateSubtree(opr->op[0])) return 1;
				if (generateSubtree(opr->op[1])) return 1;
			}
			break;
	}
	return 0;
}

void print_append(char* s, int space) {
	if (newLine) {
		fprintf(yyout, "\n");
		print_indent();
		newLine = 0;
		fprintf(yyout, "%s", s);
	} else {
		if (space)
			fprintf(yyout, " ");
		fprintf(yyout, "%s", s);
	}
}

void print_append_int(int value, int space) {
	if (newLine) {
		fprintf(yyout, "\n");
		print_indent();
		newLine = 0;
		fprintf(yyout, "%d", value);
	} else {
		if (space)
			fprintf(yyout, " ");
		fprintf(yyout, "%d", value);
	}
}

void print_indent() {
	int i;
	for (i=0; i<indent; i++) {
		fprintf(yyout, "\t");
	}
}

void print_header() {
	print_line("#include <AvlVisualizer.h>");
	print_line("#include <AvlTypes.h>");
	print_line("#include <condition_variable>");
	print_line("#include <cstdlib>");
	print_line("");

	print_line("AvlVisualizer *__avl__vi = NULL;");
	print_line("bool __avl__ready() { return __avl__vi != NULL; }");
	print_line("std::mutex __avl_mtx;");
	print_line("std::condition_variable_any __avl__cv;");
	print_line("");

	print_line("void __avl__display(int argc, char **argv)");
	print_line("{");
	print_line("\t__avl_mtx.lock();");
	print_line("\t__avl__vi = new AvlVisualizer(argc, argv);");
	print_line("\t__avl__cv.notify_one();");
	print_line("\t__avl_mtx.unlock();");
	print_line("");
	print_line("\t__avl__vi->show();");
	print_line("}");
	print_line("");
}

void print_line(const char *line) {
	fprintf(yyout, "%s\n", line);
}

void freeTree(nodeType* node) {
	int i;
	switch (node->type) {
		case CHARCON_NODE:
			free(node->charCon.value);
			break;
		case STRLIT_NODE:
			free(node->strLit.value);
			break;
		case ID_NODE:
			free(node->id.value);
			break;
		case MATHOP_NODE:
			free(node->mathOp.value);
			break;
		case OPERATOR_NODE:
			for (i=0; i<node->opr.numOperands; i++) {
				freeTree(node->opr.op[i]);
			}
			free(node->opr.op);
			break;
		default:
			break;
	}
	free(node);
}
