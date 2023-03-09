#include "Variables_void.h"
#include <stddef.h>
#include <string.h>
#include <malloc.h>

vvariable* vvariable_alloc()
{
	vvariable* v = (vvariable*)malloc(sizeof(vvariable));
	if (!v) return NULL;
	v->value = 0;
	v->name = "";
	return v;
}

vvariable* vvariable_init(char* name, void* value)
{
	vvariable* v = (vvariable*)malloc(sizeof(vvariable));
	if (!v) return NULL;
	v->value = value;
	v->name = name;
	return v;
}

// ¬озвращает 1, если одинаковы, и 0, если различны
int vvariable_compare_name(const vvariable* v1, const vvariable* v2)
{
	if (strcmp(v1->name, v2->name)) return 0;
	return 1;
}

int vvariable_compare(const vvariable* v1, const vvariable* v2)
{
	if (strcmp(v1->name, v2->name) &&
		v1->value != v2->value) return 0;
	return 1;
}

void vvariable_free(vvariable* v)
{
	free(v);
}