#include "MathPMI_Debug.h";
#include "Tokens.h"

void show_queue(queue* q);
void shunting_yard_dbg()
{
	printf("¬ведите строку: \n");
	char* ic = "10+3-10+gcd(3,9)-10*2+(3-6)";
	printf("%s\n", ic);
	queue* res = get_Reverse_Polish_Notation(ic);
	show_queue(res);
	int r = get_RPN_result(res);
	printf("–езультат = %d\n", r);
	show_queue(res);
}

void show_queue(queue* q) 
{
	queue* c = queue_clone(q);
	token* e = token_alloc();
	while (queue_pop(c, &e))
	{
		printf("%s ", e->name);
	}
	printf("\n");
}