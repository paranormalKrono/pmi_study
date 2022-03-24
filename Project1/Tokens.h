#ifndef	_TOKENS_H_
#define	_TOKENS_H_

typedef enum token_type { Unknown, Number, Operator, LPar, RPar, Function, Comma } token_type;

typedef struct token
{
	char* name;
	enum token_type type;
} token;

token* token_alloc();
token* token_init(char* name, token_type type);
void token_free(token* t);

#endif // _TOKENS_H_