#ifndef	__MENUS_H__
#define	__MENUS_H__

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <implot.h>
#include <string>
#include <vector>

#include "RPN_Interface.h"

#include "Node_menu.h"

void MainMenu(); 

void NodeMenu_();
void ComputationalMenu();

void LinearSystemsMenu();
void DifferentialMenu();
void IntegrationMenu();

void CauchyMenu();
void BoundaryMenu();


void IntegrationEquationsMenu();
void Integration();
void IntegrationMenu(double end, double begin,
	variable** variables, std::vector<queue*> function_rpns, std::vector<std::string> function_names, int* main_function_index);


void concole_clear();
int console_main();

#endif // __MENUS_H__