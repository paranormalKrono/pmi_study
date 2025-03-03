#ifndef	__INTEGRATION_SYSTEMS_H__
#define	__INTEGRATION_SYSTEMS_H__
#pragma once

#define _USE_MATH_DEFINES

#include "MainApp.h"
#include "LinearSystems.h"
#include "RPN_Interface.h"

#include <stdio.h>
#include <math.h>

#include <imgui_stdlib.h>
#include <solvers.h>

#include <cmath>
#include <cstdlib>
#include <format>
#include <vector>

extern "C"
{
#include "Tokens.h"
#include "Lessons/sq/queue_list.h"
#include "Shunting_yard.h"
#include "numerical_integration.h"
#include "non_linear_equation.h"
}

#pragma execution_character_set("utf-8")

#endif // __INTEGRATION_SYSTEMS_H__