#ifndef ST_LIST_H_
#define ST_LIST_H_

#include "sym_table.h"

struct st_node
{
	struct sym_table *st;
	struct st_node *prev;
	struct st_node *next;
};

struct st_list
{
	struct st_node *first;
	struct st_node *last;
};

/* initialize a list for symbol tables */
void st_list_init(struct st_list *sl);

/* destroy a list for symbol tables */
void st_list_destroy(struct st_list *sl);

/* add a symbol table to the head of a list */
void st_list_add(struct st_list *sl, struct sym_table *st);

/* add a symbol table to the end of a list */
void st_list_add_tail(struct st_list *sl, struct sym_table *st);

/* delete a symbol table from the head of a list */
void st_list_del(struct st_list *sl);

/* delete a symbol table from the end of a list */
void st_list_del_tail(struct st_list *sl);

/* return the head of a list */
struct sym_table *st_list_head(const struct st_list *sl);

/* return the end of a list */
struct sym_table *st_list_tail(const struct st_list *sl);

/* search for an identifier in a list,
 * return a pointer if the id is found,
 * return NULL otherwise*/
struct identifier *
st_list_find(const struct st_list *st, const char *id);

#endif /* ST_LIST_H_ */
