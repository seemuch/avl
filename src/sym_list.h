#ifndef SYM_LIST_H_
#define SYM_LIST_H_

#include "syntax_tree.h"

/* the max length of any identifier is SYM_LEN - 1 */
//#define SYM_LEN 32

struct identifier
{
	char* name;
	/* more attributes to be added */
	varTypeEnum type;
	varTypeEnum* args;
};

struct identifier_node
{
	struct identifier id;
	struct identifier_node *prev;
	struct identifier_node *next;
};

struct sym_list
{
	struct identifier_node *first;
	struct identifier_node *last;
};

/* initialize a symbol list */
void sym_list_init(struct sym_list *sl);

/* destroy a symbol list */
void sym_list_destroy(struct sym_list *sl);

/* add an identifier to the head of a list */
void sym_list_add(struct sym_list *sl, const struct identifier *id);

/* add an identifier to the tail of a list */
void sym_list_add_tail(struct sym_list *sl, const struct identifier *id);

/* delete an identifier */
void sym_list_del(struct sym_list *sl, const char *id);

/* search for an identifier in a list,
 * return a pointer if the id is found,
 * return NULL otherwise */
struct identifier *
sym_list_find(const struct sym_list *sl, const char *id);

#endif /* SYM_LIST_H_ */
