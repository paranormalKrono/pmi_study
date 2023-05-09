#include "MathPMI_Debug.h"
#include "computational_mathematics.h"
#include "dbg_computational_mathematics.h"
#include <math.h> 
#include <float.h>

char* function3_view()
{
	return "exp(3*x)";
}
char* function3_deriative_view()
{
	return "3*exp(3*x)";
}
char* function3_second_deriative_view()
{
	return "9*exp(3*x)";
}

variable** function3_variables(int* count)
{
	variable** variables = malloc(sizeof(variable*) * 1);
	if (!variables) return NULL;
	variables[0] = variable_init("x", 0);
	*count = 1;

	return variables;
}

void numerical_differentiation_dbg()
{
	printf_s(function3_view());
	printf_s("\n");
	printf_s(function3_deriative_view());
	printf_s("\n");
	printf_s(function3_second_deriative_view());
	printf_s("\n");
	int variables_count;
	variable** variables = function3_variables(&variables_count);
	queue* rpn = get_Reverse_Polish_Notation(function3_view());
	queue* rpn_deriative = get_Reverse_Polish_Notation(function3_deriative_view());
	queue* rpn_second_deriative = get_Reverse_Polish_Notation(function3_second_deriative_view());

	printf_s("Вводите числа в виде 1 или 0,1\n");

	line_segment** function_pairs;
	line_segment* segment;
	double x0, h, real_first_deriative, real_second_deriative;
	double* first_deriatives, *second_deriatives;
	int argument_values_count;
	char answer;
	do
	{
		answer = '0';

		printf_s("\n----  ||  ----\n");

		printf_s("Введите начальную точку (x0): ");
		scanf_s("%lf", &x0);

		printf_s("Введите отступы между точками дифференцирования (h): ");
		scanf_s("%lf", &h);

		argument_values_count = input_pairs_count();

		segment = line_segment_init(x0, x0 + h * (argument_values_count-1));

		function_pairs = get_RPN_function_pairs(0, segment, argument_values_count, rpn, variables);


		printf_s("\n[%10s\t\t %10s\t\t %10s\t\t %10s\t\t %20s\t\t %10s\t\t %10s\t\t %20s]\n",
			"x", "f(x)", "fc'(x)", "f'(x)", "|fc'(x) - f'(x)|", "fc''(x)", "f''(x)", "|fc''(x) - f''(x)|"
		);

		first_deriatives = get_deriative_list(h, function_pairs, argument_values_count);
		second_deriatives = get_second_deriative_list(h, function_pairs, argument_values_count);

		variables[0]->value = x0;
		for (int i = 0; i < argument_values_count; ++i)
		{
			real_first_deriative = get_RPN_result(rpn_deriative, variables, variables_count);
			real_second_deriative = get_RPN_result(rpn_second_deriative, variables, variables_count);
			printf_s("[%10.3lf\t\t %10.3lf\t\t %10.5lf\t\t %10.5lf\t\t %20.4E\t\t %10.5lf\t\t %10.5lf\t\t %20.4E]\n", function_pairs[i]->A, function_pairs[i]->B, 
				first_deriatives[i], real_first_deriative,
				fabs(first_deriatives[i] - real_first_deriative),
				second_deriatives[i], real_second_deriative,
				fabs(second_deriatives[i] - real_second_deriative));
			variables[0]->value += h;
		}



		for (int i = 0; i < argument_values_count; ++i)
		{
			free(function_pairs[i]);
		}
		free(first_deriatives);
		free(second_deriatives);
		free(function_pairs);
		free(segment);

		printf_s("%s", "Вы хотите выбрать другие параметры? Y/N (Да/Нет)\n");
		while (answer != 'Y' && answer != 'N')
			scanf_s("%c", &answer, 1);
	} while (answer == 'Y');

}