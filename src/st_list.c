#include <stdlib.h>
#include <string.h>
#include "st_list.h"

void st_list_init(struct st_list *sl)
{
	sl->first = NULL;
	sl->last = NULL;
}

void st_list_destroy(struct st_list *sl)
{
	while (sl->first)
	{
		struct st_node *node = sl->first->next;
		free(sl->first);
		sl->first = node;
	}

	sl->last = NULL;
}

void st_list_add(struct st_list *sl, struct sym_table *st)
{
	struct st_node *node = (struct st_node *)malloc(sizeof(struct st_node));
	memset(node, 0, sizeof(struct st_node));
	node->st = st;

	if (!sl->first)
	{
		sl->first = node;
		sl->last = node;
	}
	else
	{
		node->next = sl->first;
		sl->first->prev = node;
		sl->first = node;
	}
}

void st_list_add_tail(struct st_list *sl, struct sym_table *st)
{
	struct st_node *node = (struct st_node *)malloc(sizeof(struct st_node));
	memset(node, 0, sizeof(struct st_node));
	node->st = st;

	if (!sl->first)
	{
		sl->first = node;
		sl->last = node;
	}
	else
	{
		node->prev = sl->last;
		sl->last->next = node;
		sl->last = node;
	}
}

void st_list_del(struct st_list *sl)
{
	if (!sl->first)
		return;

	if (sl->first == sl->last)
	{
		free(sl->first);
		sl->first = NULL;
		sl->last = NULL;
		return;
	}

	struct st_node *node = sl->first->next;
	free(sl->first);
	node->prev = NULL;
	sl->first = node;
}

void st_list_del_tail(struct st_list *sl)
{
	if (!sl->last)
		return;

	if (sl->first == sl->last)
	{
		free(sl->last);
		sl->first = NULL;
		sl->last = NULL;
		return;
	}

	struct st_node *node = sl->last->prev;
	free(sl->last);
	node->next = NULL;
	sl->last = node;
}

struct sym_table *st_list_head(const struct st_list *sl)
{
	return sl->first->st;
}

struct sym_table *st_list_tail(const struct st_list *sl)
{
	return sl->last->st;
}

struct identifier *st_list_find(const struct st_list *st, const char *id)
{
	struct st_node *node = st->first;
	struct identifier *ret = NULL;

	while (node)
	{
		ret = sym_table_find(node->st, id);
		if (ret)
			return ret;

		node = node->next;
	}

	return ret;
}
