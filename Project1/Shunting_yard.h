#ifndef	_SHUNTING_YARD_H_
#define	_SHUNTING_YARD_H_
#include "Lessons/sq/stack_list.h"
#include "Lessons/sq/queue_list.h"
#include "variables.h"

// Алгоритм сортировочной станции

queue* get_Reverse_Polish_Notation(char* str);

double get_RPN_result(const queue * rpn, const variable** variables, int variables_count);

#endif // _SHUNTING_YARD_H_