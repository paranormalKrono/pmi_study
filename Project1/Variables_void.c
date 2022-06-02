#include "Variables_void.h"
#include <stddef.h>
#include <string.h>

vvariable* vvariable_alloc()
{
	vvariable* v = (vvariable*)malloc(sizeof(vvariable));
	v->value = 0;
	v->name = "";
}

vvariable* vvariable_init(char* name, void* value)
{
	vvariable* v = (vvariable*)malloc(sizeof(vvariable));
	v->value = value;
	v->name = name;
}

// ���������� 1, ���� ���������, � 0, ���� ��������
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