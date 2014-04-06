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
		struct identifier *iden = sl->first->next;
		free(sl->first);
		sl->first = iden;
	}

	sl->last = NULL;
}

void sym_list_add(struct sym_list *sl, const char *id)
{
	if (sym_list_find(sl, id) == 0)
		return;

	struct identifier *iden = (struct identifier *)malloc(sizeof(struct identifier));
	memset(iden, 0, sizeof(struct identifier));
	strcpy(iden->name, id);

	if (!sl->first)
	{
		sl->first = iden;
		sl->last = iden;
	}
	else
	{
		iden->next = sl->first;
		sl->first->prev = iden;
		sl->first = iden;
	}
}

void sym_list_add_tail(struct sym_list *sl, const char *id)
{
	if (sym_list_find(sl, id) == 0)
		return;

	struct identifier *iden = (struct identifier *)malloc(sizeof(struct identifier));
	memset(iden, 0, sizeof(struct identifier));
	strcpy(iden->name, id);

	if (!sl->first)
	{
		sl->first = iden;
		sl->last = iden;
	}
	else
	{
		iden->prev = sl->last;
		sl->last->next = iden;
		sl->last = iden;
	}
}

void sym_list_del(struct sym_list *sl, const char *id)
{
	if (sym_list_find(sl, id) < 0)
		return;

	if (sl->first == sl->last)
	{
		free(sl->first);
		sl->first = NULL;
		sl->last = NULL;
		return;
	}

	struct identifier *iden = sl->first;
	
	while (iden)
	{
		if (strcmp(iden->name, id) == 0)
		{
			if (iden == sl->first)
			{
				iden->next->prev = NULL;
				sl->first = iden->next;
			}
			else if (iden == sl->last)
			{
				iden->prev->next = NULL;
				sl->last = iden->prev;
			}
			else
			{
				iden->prev->next = iden->next;
				iden->next->prev = iden->prev;
			}
			free(iden);
			return;
		}
		else
			iden = iden->next;
	}
}

int sym_list_find(const struct sym_list *sl, const char *id)
{
	struct identifier *iden = sl->first;

	while (iden)
	{
		if (strcmp(iden->name, id) == 0)
			return 0;

		iden = iden->next;
	}

	return -1;
}
