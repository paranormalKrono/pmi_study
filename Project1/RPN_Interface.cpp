#include "RPN_Interface.h"

int formula_rpn::recalculate_rpn()
{
	if (isActualRPN) return 0;

	if (rpn)
		queue_free(rpn);

	rpn = get_Reverse_Polish_Notation(definition.data());

	isActualRPN = true;
	return 0;
}


int formula_rpn::set_arguments(std::initializer_list<double> values)
{
	if (values.size() > vars_count) return 1;

	const double* cur_v = values.begin();
	for (int i{ 0 }; i < values.size(); ++i, cur_v++)
		vars[i]->value = *cur_v;

	return 0;
}

int formula_rpn::set_argument(const int id, const double value)
{
	if (vars_count < id) return 1;

	vars[id]->value = value;

	return 0;
}


int formula_rpn::calculate(double& res)
{
	if (!isActualRPN) return 1;

	res = get_RPN_result(rpn, vars, vars_count);
	return 0;
}

int formula_rpn::calculate(double& res, std::initializer_list<double> values)
{
	if (!isActualRPN) return 1;

	set_arguments(values);

	res = get_RPN_result(rpn, vars, vars_count);
	return 0;
}

variable** formula_rpn::get_standard_variables(const int& count, 
	const char* name1, const char* name2)
{
	variable** variables = (variable**)malloc(sizeof(variable*) * count);
	if (!variables) return NULL;

	variables[0] = variable_init(name1, 0);
	if (count > 1)
		variables[1] = variable_init(name2, 0);
	if (count > 2)
		variables[2] = variable_init("C", 0);
	for (int i = 3; i < count; ++i)
		variables[i] = variable_init((new std::string("C" + std::to_string(i)))->c_str(), 0);

	return variables;
}

variable** formula_rpn::get_standard_variable()
{
	variable** variables = (variable**)malloc(sizeof(variable*) * 1);
	if (!variables) return NULL;
	variables[0] = variable_init("x", 0);
	return variables;
}

std::string formula_rpn::to_string()
{
	return formula_rpn::queue_to_string(this->rpn);
}

std::string formula_rpn::queue_to_string(queue* q)
{
	printf_s("--- Обратная польская нотация ---\n");
	std::string s = "";
	queue* c = queue_clone(q);
	element_t e;
	token* t = token_alloc();
	while (queue_pop(c, &e))
	{
		t = static_cast<token*>(e);
		s += " ";
		s += t->name;
		printf_s("%s ", t->name);
	}
	printf_s("\n\n");
	return s;
}