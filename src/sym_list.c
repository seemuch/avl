#include <string.h>
#include <stdlib.h>
#include "sym_list.h"

void sym_list_init(struct sym_list *sl)
{
	sl->first = NULL;
	sl->last = NULL;
}

void sym_list_destroy(struct sym_list *sl)
{
	while (sl->first)
	{
		struct identifier_node *node = sl->first->next;
		free(sl->first);
		sl->first = node;
	}

	sl->last = NULL;
}

void sym_list_add(struct sym_list *sl, const struct identifier *id)
{
	if (sym_list_find(sl, id->name) != NULL)
		return;

	struct identifier_node *node = (struct identifier_node *)malloc(sizeof(struct identifier_node));
	memset(node, 0, sizeof(struct identifier_node));
	memcpy(&node->id, id, sizeof(struct identifier));

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

void sym_list_add_tail(struct sym_list *sl, const struct identifier *id)
{
	if (sym_list_find(sl, id->name) != NULL)
		return;

	struct identifier_node *node = (struct identifier_node *)malloc(sizeof(struct identifier_node));
	memset(node, 0, sizeof(struct identifier_node));
	memcpy(&node->id, id, sizeof(struct identifier));

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

void sym_list_del(struct sym_list *sl, const char *id)
{
	if (sym_list_find(sl, id) == NULL)
		return;

	if (sl->first == sl->last)
	{
		free(sl->first);
		sl->first = NULL;
		sl->last = NULL;
		return;
	}

	struct identifier_node *node = sl->first;
	
	while (node)
	{
		if (strcmp(node->id.name, id) == 0)
		{
			if (node == sl->first)
			{
				node->next->prev = NULL;
				sl->first = node->next;
			}
			else if (node == sl->last)
			{
				node->prev->next = NULL;
				sl->last = node->prev;
			}
			else
			{
				node->prev->next = node->next;
				node->next->prev = node->prev;
			}
			free(node);
			return;
		}
		else
			node = node->next;
	}
}

struct identifier *sym_list_find(const struct sym_list *sl, const char *id)
{
	struct identifier_node *node = sl->first;

	while (node)
	{
		if (strcmp(node->id.name, id) == 0)
			return &node->id;

		node = node->next;
	}

	return NULL;
}
