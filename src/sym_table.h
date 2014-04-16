#ifndef SYM_TABLE_H_
#define SYM_TABLE_H_

#include "sym_list.h"

#define SYM_TABLE_SIZE 701

struct sym_table
{
	struct sym_list htable[SYM_TABLE_SIZE];
};

/* initialize a symbol table */
void sym_table_init(struct sym_table *st);

/* destroy a symbol table */
void sym_table_destroy(struct sym_table *st);

/* add an identifier to a symbol table */
void sym_table_add(struct sym_table *st, const struct identifier *id);

/* delete an identifier */
void sym_table_del(struct sym_table *st, const char *id);

/* search for an identifier in a symbol table,
 * return a pointer if the id is found,
 * return NULL otherwise */
struct identifier *sym_table_find(const struct sym_table *st, const char *id);

#endif /* SYM_TABLE_H_ */
