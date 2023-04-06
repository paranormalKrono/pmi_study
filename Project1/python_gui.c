
#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <direct.h>

#include "python_gui.h"

void create_window() 
{

	//PyObject* pInt;

	//Py_Initialize();

	//PyRun_SimpleString("print('Hello World from Embedded Python!!!')");

	//Py_Finalize();

	//printf("\nPress any key to exit...\n");
	//if (!_getch()) _getch();
	//return 0;


	PyObject* pModule, * pFunc, * pArgs, * pValue; // pointers to Python objects
	double x, y;
	x = 2.0; y = 3.0; // values to be added
	Py_Initialize(); // initialize Python

	PyRun_SimpleString("import sys");
	PyRun_SimpleString("import os");
	PyRun_SimpleString("sys.path.append(os.getcwd())");

	// import module
	pModule = PyImport_Import(PyUnicode_FromString("Py_graphlib"));
	PyErr_Print();
	if (!pModule) printf_s("Failed to load <Py_graphlib.py>");
	// get reference
	pFunc = PyObject_GetAttrString(pModule, "create_graph");
	pArgs = PyTuple_New(2); // define tuple of 2 arguments
	PyTuple_SetItem(pArgs, 0, PyFloat_FromDouble(x)); // set 1st argument
	PyTuple_SetItem(pArgs, 1, PyFloat_FromDouble(y)); // set 2nd argument
	pValue = PyObject_CallObject(pFunc, pArgs); // call Python function
	printf_s("x + y = %lf", PyFloat_AsDouble(pValue));

	Py_DECREF(pArgs); // clean up
	Py_DECREF(pFunc);
	Py_DECREF(pModule);
	Py_Finalize(); // finish Python
}