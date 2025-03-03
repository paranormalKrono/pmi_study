#pragma once

#include <string>

extern "C"
{
#include "Lessons/sq/queue_list.h"
#include "variables.h"
#include "Shunting_yard.h"
#include "Tokens.h"
}

class formula_rpn
{
public:
	std::string definition;

	formula_rpn(std::string formula_definition, int variables_count, variable** variables) :
		definition{ formula_definition }, vars_count{ variables_count }, vars{ variables },
		rpn{ nullptr }, isActualRPN{ false }
	{
		recalculate_rpn();
	}

	~formula_rpn()
	{
		delete[] vars;
	}

	int recalculate_rpn();
	int set_arguments(std::initializer_list<double> values);
	int set_argument(const int id, const double value);
	int calculate(double& res);
	int calculate(double& res, std::initializer_list<double> values);

	std::string to_string();
	static std::string queue_to_string(queue* q);

	static variable** get_standard_variables(const int& count, 
		const char* name1, const char* name2);
	static variable** get_standard_variable();

private:
	queue* rpn;
	bool isActualRPN;

	int vars_count;
	variable** vars;
};