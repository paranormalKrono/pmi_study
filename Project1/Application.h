#ifndef	_APPLICATION_H_
#define	_APPLICATION_H_

#include <imgui.h>
#include <implot.h>
#include <string>
#include <vector>


extern "C"
{
	#include "Lessons/sq/queue_list.h"
	#include "variables.h"
	//#include <Python.h>
	//#include "python_gui.h"
}

namespace MyApp 
{

	void Initialisation();
	void RenderUI();

	void ShowMenu();
	void ComputationalMathematics();
	void DifferentialMathematics();

	void IntegrationMenu(double end, double begin,
		variable** variables, std::vector<queue*> function_rpns, std::vector<std::string> function_names, int* main_function_index);

	void CauchyMenu(double yx0, variable** variables, queue* function_rpn);
}

#endif // _APPLICATION_H_