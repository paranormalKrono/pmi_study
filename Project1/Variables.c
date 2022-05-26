#include "Variables.h"
#include <stddef.h>
#include <string.h>

variable* variable_alloc()
{
	variable* v = (variable*)malloc(sizeof(variable));
	v->value = 0;
	v->name = "";
}

variable* variable_init(char* name, int value)
{
	variable* v = (variable*)malloc(sizeof(variable));
	v->value = value;
	v->name = name;
}

// ¬озвращает 1, если одинаковы, и 0, если различны
int variable_compare_name(const variable* v1, const variable* v2)
{
	if (strcmp(v1->name, v2->name)) return 0;
	return 1;
}

int variable_compare(const variable* v1, const variable* v2)
{
	if (strcmp(v1->name, v2->name) &&
		v1->value != v2->value) return 0;
	return 1;
}

void variable_free(variable* v)
{
	free(v);
}