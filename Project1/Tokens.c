#include "Tokens.h"
#include <stddef.h>

token* token_alloc()
{
	token* t = (token*)malloc(sizeof(token));
	t->name = NULL;
	t->type = Unk;
}

token* token_init(char* name, token_type type) 
{
	token* t = (token*)malloc(sizeof(token));
	t->name = name;
	t->type = type;
}

void token_free(token* t)
{
	free(t->name);
	free(t);
}