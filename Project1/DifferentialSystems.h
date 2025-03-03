#ifndef	__DIFFERENTIAL_SYSTEMS_H__
#define	__DIFFERENTIAL_SYSTEMS_H__
#pragma once

#define _USE_MATH_DEFINES

#include "MainApp.h"
#include "LinearSystems.h"

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
}

#endif // __DIFFERENTIAL_SYSTEMS_H__