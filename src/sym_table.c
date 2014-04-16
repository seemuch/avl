#include <string.h>
#include "sym_table.h"

size_t hash(const char *id)
{
	size_t num = 0;
	size_t len = strlen(id);
	size_t i;

	for (i = 0; i < len; i++)
	{
		num *= 64;
		num %= SYM_TABLE_SIZE;

		if (id[i] == '_')
			num += 0;
		else if ('a' <= id[i] && id[i] <= 'z')
			num += (unsigned int)(id[i] - 'a' + 1);
		else if ('A' <= id[i] && id[i] <= 'Z')
			num += (unsigned int)(id[i] - 'A' + 27);
		else /* '0' <= id[i] && id[i] <= '9' */
			num += (unsigned int)(id[i] - '0' + 53);
	}

	return num % SYM_TABLE_SIZE;
}

void sym_table_init(struct sym_table *st)
{
	size_t i;

	for (i = 0; i < SYM_TABLE_SIZE; i++)
		sym_list_init(&st->htable[i]);
}

void sym_table_destroy(struct sym_table *st)
{
	size_t i;

	for (i = 0; i < SYM_TABLE_SIZE; i++)
		sym_list_destroy(&st->htable[i]);
}

void sym_table_add(struct sym_table *st, const struct identifier *id)
{
	size_t idx = hash(id->name);

	sym_list_add(&st->htable[idx], id);
}

void sym_table_del(struct sym_table *st, const char *id)
{
	size_t idx = hash(id);

	sym_list_del(&st->htable[idx], id);
}

struct identifier *sym_table_find(const struct sym_table *st, const char *id)
{
	size_t idx = hash(id);

	return sym_list_find(&st->htable[idx], id);
}
