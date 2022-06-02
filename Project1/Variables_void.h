#ifndef	_VARIABLES_VOID_H_
#define	_VARIABLES_VOID_H_

typedef struct vvariable
{
	char* name;
	void* value;
} vvariable;

vvariable* vvariable_alloc();
vvariable* vvariable_init(char* name, void* value);
int vvariable_compare_name(const vvariable* v1, const vvariable* v2);
int vvariable_compare(const vvariable* v1, const vvariable* v2);
void vvariable_free(vvariable* v);


#endif // _VARIABLES_VOID_H_