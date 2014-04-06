#ifndef SYM_LIST_H_
#define SYM_LIST_H_

/* the max length of any identifier is SYM_LEN - 1 */
#define SYM_LEN 32

struct identifier
{
	char name[SYM_LEN];
	struct identifier *prev;
	struct identifier *next;
};

struct sym_list
{
	struct identifier *first;
	struct identifier *last;
};

/* initialize a symbol list */
void sym_list_init(struct sym_list *sl);

/* destroy a symbol list */
void sym_list_destroy(struct sym_list *sl);

/* add an identifier to the head of a list */
void sym_list_add(struct sym_list *sl, const char *id);

/* add an identifier to the tail of a list */
void sym_list_add_tail(struct sym_list *sl, const char *id);

/* delete an identifier */
void sym_list_del(struct sym_list *sl, const char *id);

/* search for an identifier in a list,
 * return 0 if the id is found, return -1 otherwise */
int sym_list_find(const struct sym_list *sl, const char *id);

#endif /* SYM_LIST_H_ */
