#ifndef	_APPLICATION_H_
#define	_APPLICATION_H_

#include <imgui.h>
#include <implot.h>
#include <string>


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

	void IntegrationMenu(int antiderivatives_count, double end, double begin, variable** variables, queue** function_rpns, int fixed_function_count, int* main_function_index);
}

#endif // _APPLICATION_H_