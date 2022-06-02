#ifndef	_SHUNTING_YARD_H_
#define	_SHUNTING_YARD_H_
#include "Lessons/sq/stack_list.h"
#include "Lessons/sq/queue_list.h"
#include "Variables_void.h"

// �������� ������������� �������

queue* get_Reverse_Polish_Notation(char* str);

void* get_RPN_result(const queue const* rpn, const vvariable* vars, int count);

#endif // _SHUNTING_YARD_H_