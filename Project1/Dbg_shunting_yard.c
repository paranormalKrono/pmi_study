#include "MathPMI_Debug.h";
#include "Tokens.h"

void shunting_yard_dbg()
{
	printf("__SHUNTING_YARD_DEBUG__\n");
	printf("¬ведите строку: \n");
	//char* ic = "10+23+gcd(32,24+1)";
	//char* ic = "10+3-10+gcd(3,9)-10*2+(3-6)";
	char* mathematical = "25+35*(x-5)*(y+5) + exp(2) + pow(2, 3.002)";

	variable** vars = malloc(sizeof(variable*) * 2);
	if (!vars) return;
	double x = 2, y = 4;
	vars[0] = variable_init("x", x);
	vars[1] = variable_init("y", y);

	printf("%s\n", mathematical);
	queue* res = get_Reverse_Polish_Notation(mathematical);
	//show_queue(res);
	double r = get_RPN_result(res, vars, 2);

	printf("–езультат = %lf\n", r);
	//show_queue(res);
}

