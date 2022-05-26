#ifndef	_VARIABLES_H_
#define	_VARIABLES_H_

typedef struct variable
{
	char* name;
	int value;
} variable;

variable* variable_alloc();
variable* variable_init(char* name, int value);
int variable_compare_name(const variable* v1, const variable* v2);
int variable_compare(const variable* v1, const variable* v2);
void variable_free(variable* v);


#endif // _VARIABLES_H_