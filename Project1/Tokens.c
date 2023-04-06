#include "Tokens.h"
#include <malloc.h>
#include <memory.h>


token* token_alloc()
{
	token* t = (token*)malloc(sizeof(token));
	if (!t) return NULL;
	t->name = NULL;
	t->type = Unk;
	return t;
}

token* token_init(char* name, token_type type) 
{
	token* t = (token*)malloc(sizeof(token));
	if (!t) return NULL;
	t->name = name;
	t->type = type;
	return t;
}

void token_free(token* t)
{
	free(t->name);
	free(t);
}